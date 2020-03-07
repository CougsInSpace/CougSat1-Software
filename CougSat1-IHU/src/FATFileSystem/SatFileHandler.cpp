#include "SatFileHandler.h"
#include <algorithm>
#include <fstream>
#include <sstream>

SatFileHandler::SatFileHandler(PinName mosi, PinName miso, PinName sclk,
                               PinName cs, PinName cd, uint64_t hz, bool crc_on,
                               bool debug)
        : debug(debug), needsReformat(false), current(0), priority(0)
{
        hwo = std::make_unique<HardwareOptions>();

        hwo->mosi = mosi;
        hwo->miso = miso;
        hwo->sclk = sclk;
        hwo->cs = cs;
        hwo->freq = hz;
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
        std::string fileName = "/fs/" + filenameBase;

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
        std::ifstream file("/fs/" + fileNameFull);
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
        DIR *d = opendir("/fs/");
        struct dirent ent;
        std::string directory = "/fs/" + dir;
        if (d->read(&ent)) {
                // DIR *temp = opendir(directory.c_str());
                fs->remove(directory.c_str());
        }

        this->priority++;
}

void SatFileHandler::check()
{
        uint8_t full[512];
        std::fill_n(full, 512, 0xFF);
        uint8_t blockData[512] = {0}, tmp[512] = {0};
        bd_size_t readSize = sdbd->get_read_size();
        bd_size_t num_blocks = sdbd->size() / readSize;
        uint32_t numBad = 0;
        if (debug)
                pc->printf("SD Block Count: %lu\r\n", num_blocks);
        for (bd_size_t i = 0; i < num_blocks; i += 512) {
                sdbd->read(tmp, i, readSize);
                sdbd->program(full, i, readSize);
                sdbd->read(blockData, i, readSize);
                if (!compareArrays(blockData, full, 512)) {
                        numBad++;
                }
                sdbd->program(&tmp, i, readSize);
        }
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
                                               hwo->cs, hwo->freq, hwo->crc_on);
        int status = sdbd->init();
        if (debug) {
                int cardDetected = cardDetect->read();
                pc->printf("SD Card Detected: \r\n");
                pc->printf("%s\r\n", cardDetected ? "True" : "False");
                pc->printf("SD Device Size: %lu\r\n", sdbd->size());
                pc->printf("SD Read Size: %lu\r\n", sdbd->get_read_size());
                pc->printf("SD Program Size: %lu\r\n",
                           sdbd->get_program_size()); // write size,
                                                      // basically
                pc->printf("SD Erase Size: %lu\r\n", sdbd->get_erase_size());
                ;
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
                pc->printf("Filesystem Reformat: \r\n");
                pc->printf("%s\r\n", status ? "Failed" : "Success");
        }
}

void SatFileHandler::mount()
{
        int status = fs->mount(sdbd.get());
        if (debug) {
                pc->printf("Filesystem Mount: \r\n");
                pc->printf("%s\r\n", status ? "Failed" : "Success");
        }
}

void SatFileHandler::unmount()
{
        int status = fs->unmount();
        if (debug) {
                pc->printf("Filesystem Unmount: \r\n");
                pc->printf("%s\r\n", status ? "Failed" : "Success");
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

size_t SatFileHandler::freeSpace()
{
        struct statvfs fsinfo;
        fs->statvfs("/fs/", &fsinfo);
        return fsinfo.f_bfree * fsinfo.f_bsize;
}

size_t SatFileHandler::blockDeviceSize()
{
        return sdbd->size();
}
