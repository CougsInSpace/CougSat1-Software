#include "SatFileHandler.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>

constexpr char SatFileHandler::rootDirectory[];
constexpr uint64_t SatFileHandler::frequency;

SatFileHandler::SatFileHandler(PinName mosi, PinName miso, PinName sclk,
                               PinName cs, PinName cd, bool crc_on, bool debug)
        : debug(debug), needsReformat(false), current(0), priority(0)
{
        hwo = std::make_unique<HardwareOptions>();

        hwo->mosi = mosi;
        hwo->miso = miso;
        hwo->sclk = sclk;
        hwo->cs = cs;
        hwo->crc_on = crc_on;
        hwo->cd = cd;
}

SatFileHandler::~SatFileHandler()
{
        unmount();
        sdbd->deinit();
}

void SatFileHandler::writef(std::string filenameBase, const char *message)
{

        std::fstream file;
        std::string fileName = rootDirectory + filenameBase;

        std::string currentString = "";

        for (int i = this->current; i; i /= 10) {
                currentString = (char)((i % 10) + '0') + currentString;
        }

        fileName += currentString + ".txt";
        file.open(fileName, std::ios::out | std::ios::app);

        if (!file.is_open()) {
                pc->printf("WRITE_ERROR: Failed to open file\r\n");
                return;
        }

        file << message << "\n";

        this->current++;
}

void SatFileHandler::write(std::string filenameBase, const std::string &message)
{
        this->writef(filenameBase, message.c_str());
}

void SatFileHandler::writeStart()
{
        while (!inputMessages.empty()) {
                std::pair<std::string, std::string> p = inputMessages.front();
                write(get<0>(p), get<1>(p));
                inputMessages.pop();
        }
}

std::string SatFileHandler::read(const std::string &fileNameFull)
{
        std::ifstream file(rootDirectory + fileNameFull);
        std::stringstream out;
        if (!file.is_open()) {
                pc->printf("READ_ERROR: File cannot be found\r\n");
                return "";
        }
        out << file.rdbuf();
        return out.str();
}

void SatFileHandler::clean(std::string dir)
{
        DIR *d = opendir(rootDirectory);
        struct dirent ent;
        std::string directory = rootDirectory + dir;
        if (d->read(&ent)) {
                // DIR *temp = opendir(directory.c_str());
                fs->remove(directory.c_str());
        }

        this->priority++;
}

void SatFileHandler::check()
{
        uint8_t full[BLOCKSIZE];
        std::fill_n(full, BLOCKSIZE, 0xFF);
        uint8_t blockData[BLOCKSIZE] = {0}, tmp[BLOCKSIZE] = {0};
        bd_size_t numBlocks = sdbd->size() / BLOCKSIZE;
        uint32_t numBad = 0;

        if (debug)
                pc->printf("SD Block Count: %lu\r\n", numBlocks);

        std::chrono::high_resolution_clock::time_point t1
                = std::chrono::high_resolution_clock::now();

        auto checkBlock = [&](bd_size_t i) {
                sdbd->read(tmp, i, BLOCKSIZE);
                sdbd->program(full, i, BLOCKSIZE);
                sdbd->read(blockData, i, BLOCKSIZE);
                if (!compareArrays(blockData, full, BLOCKSIZE)) {
                        numBad++;
                }
                sdbd->program(&tmp, i, BLOCKSIZE);
        };

        for (bd_size_t i = 0; i < numBlocks; i += BLOCKSIZE) {
                checkBlock(i);
        }

        std::chrono::high_resolution_clock::time_point t2
                = std::chrono::high_resolution_clock::now();
        std::chrono::seconds dur
                = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);

        int32_t seconds = static_cast<int64_t>(dur.count());
        pc->printf("Check took %ld seconds\r\n", seconds);
        pc->printf("Block device check: Done\r\n");
        pc->printf("Number of bad blocks: %lu\r\n", numBad);
}

void SatFileHandler::enqueueMessage(std::pair<std::string, std::string> message)
{
        inputMessages.push(message);
}

mbed_error_status_t SatFileHandler::init()
{
        initSerial();
        cardDetect = std::make_unique<DigitalIn>(hwo->cd, PullUp);
        int bdStat = initBlockDevice();
        int fsStat = initFilesystem();
        hwo.reset();
        return (bdStat || fsStat) ? MBED_ERROR_CODE_FAILED_OPERATION
                                  : MBED_SUCCESS;
}

mbed_error_status_t SatFileHandler::initFilesystem()
{
        fs = std::make_unique<FATFileSystem>("fs");
        int status = fs->mount(sdbd.get());
        if (status) {
                pc->printf("Failed to mount filesystem, "
                           "reformatting...\r\n");
                reformat();
                pc->printf("Reformat done\r\n");
                status = fs->mount(sdbd.get());
        }
        if (debug)
                pc->printf("FileSystem Mount: \r\n%s\r\n", strerror(-status));
        return status;
}

mbed_error_status_t SatFileHandler::initBlockDevice()
{
        sdbd = std::make_unique<SDBlockDevice>(hwo->mosi, hwo->miso, hwo->sclk,
                                               hwo->cs, frequency, hwo->crc_on);
        ThisThread::sleep_for(0.2); // a small wait for when using crc.
        int status = sdbd->init();
        if (debug) {
                int cardDetected = cardDetect->read();
                pc->printf("SD Card Detected: \r\n%s\r\n",
                           cardDetected ? "True" : "False");
                pc->printf("SD Block Device Init: \r\n%s\r\n",
                           (status == 0) ? "Success" : "Failure");
        }
        return status;
}

void SatFileHandler::initSerial()
{
        pc = std::make_unique<Serial>(SERIAL_TX, SERIAL_RX);
}

void SatFileHandler::reformat()
{
        int status = fs->reformat(sdbd.get());
        if (debug) {
                pc->printf("Filesystem Reformat: \r\n%s\r\n",
                           status ? "Failed" : "Success");
        }
}

void SatFileHandler::mount()
{
        int status = fs->mount(sdbd.get());
        if (debug) {
                pc->printf("Filesystem Mount: \r\n%s\r\n",
                           status ? "Failed" : "Success");
        }
}

void SatFileHandler::unmount()
{
        int status = fs->unmount();
        if (debug) {
                pc->printf("Filesystem Unmount: \r\n%s\r\n",
                           status ? "Failed" : "Success");
        }
}

bool SatFileHandler::compareArrays(uint8_t *arr1, uint8_t *arr2, size_t size)
{
        for (size_t i = 0; i < size; i++) {
                if (arr1[i] != arr2[i])
                        return false;
        }
        return true;
}

size_t SatFileHandler::freeSpace() const
{
        struct statvfs fsinfo;
        fs->statvfs(rootDirectory, &fsinfo);
        return fsinfo.f_bfree * fsinfo.f_bsize;
}

size_t SatFileHandler::blockDeviceSize() const
{
        return sdbd->size();
}

void SatFileHandler::writeBin(const std::string &fileName)
{
        std::fstream binToWrite(fileName, std::ios::in | std::ios::binary);
        std::fstream binOut(rootDirectory + fileName,
                            std::ios::out | std::ios::binary);
        binToWrite.seekg(0, binToWrite.end);
        size_t fileSize = binToWrite.tellg();
        binToWrite.seekg(0);

        char *buffer = new char[fileSize];
        binToWrite.read(buffer, fileSize);
        binOut.write(buffer, fileSize);

        delete[] buffer;
        binOut.flush();
}

std::fstream SatFileHandler::readBin(const std::string &fileName)
{
        return std::fstream(fileName, std::ios::out | std::ios::binary);
}