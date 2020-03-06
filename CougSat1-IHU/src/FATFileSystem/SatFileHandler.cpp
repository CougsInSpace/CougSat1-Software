#include "SatFileHandler.h"
#include <algorithm>
#include <sstream>

SatFileHandler::SatFileHandler(PinName mosi, PinName miso, PinName sclk,
                               PinName cs, uint64_t hz, bool crc_on, bool debug)
{
        hwo = std::make_unique<HardwareOptions>();

        hwo->mosi = mosi;
        hwo->miso = miso;
        hwo->sclk = sclk;
        hwo->cs = cs;
        hwo->freq = hz;
        hwo->crc_on = crc_on;

        this->debug = debug;
        this->needsReformat = false;
        this->current = 0;
        this->priority = 0;
}

SatFileHandler::~SatFileHandler()
{
        unmount();
        sdbd->deinit();
}

bool SatFileHandler::writef(std::string filenameBase, const char *message)
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
                if (debug) {
                        pc->printf("ERROR failed to open file");
                }
                return false;
        }

        file << message << std::endl;
        file.flush();

        file.close();

        this->current++;
        return true;
}

bool SatFileHandler::write(std::string filenameBase, std::string &message)
{
        return this->writef(filenameBase, message.c_str());
}

bool SatFileHandler::writeStart()
{
        bool t = false;
        while (!inputMessages.empty()) {
                std::pair<std::string, std::string> p = inputMessages.front();
                t = write(get<0>(p), get<1>(p));
                inputMessages.pop();
        }
        return t;
}

std::string SatFileHandler::read(const std::string &fileNameFull)
{
        std::ifstream file("/fs/" + fileNameFull);
        std::stringstream out;
        if (!file.is_open()) {
                pc->printf("ERROR: File cannot be found\r\n");
                return "";
        }
        out << file.rdbuf();
        return out.str();
}

//
bool SatFileHandler::clean(std::string dir)
{
        DIR *d = opendir("/fs/");
        struct dirent ent;
        std::string directory = "/fs/" + dir;
        if (d->read(&ent)) {
                // DIR *temp = opendir(directory.c_str());
                fs->remove(directory.c_str());
        }

        this->priority++;
        return true;
}

bool SatFileHandler::check()
{
        // struct statvfs fsinfo;
        // int status = fs->statvfs("/fs/", &fsinfo);
        // if (fsinfo.f_bfree < 512 * 10 && clean("/fs/"))
        //         return true;
        // else
        //         return false;
        // return status == 0;
        uint8_t full[512];
        std::fill_n(full, 512, 0xFF);
        uint8_t blockData[512] = {0}, tmp[512] = {0};
        bd_size_t readSize = sdbd->get_read_size();
        bd_size_t num_blocks = sdbd->size() / readSize;
        if (debug)
                pc->printf("SD Block Count: %lu\r\n", num_blocks);

        for (bd_size_t i = 0; i < num_blocks; i += 512) {
                sdbd->read(tmp, i, readSize);
                sdbd->program(full, i, readSize);
                sdbd->read(blockData, i, readSize);
                if (!compareArrays(blockData, full, 512))
                        return false;
                sdbd->program(&tmp, i, readSize);
        }
        return true;
}

bool SatFileHandler::enqueueMessage(std::pair<std::string, std::string> message)
{

        // Mutex mute;

        // mute.lock();
        inputMessages.push(message);
        return true;
        // mute.unlock();
}

void SatFileHandler::init()
{
        if (debug)
                initSerial();
        initBlockDevice();
        initFilesystem();
        hwo.reset();
}

void SatFileHandler::initFilesystem()
{
        fs = std::make_unique<FATFileSystem>("fs");
        int status = fs->mount(sdbd.get());
        if (status) {
                pc->printf("Failed to mount filesystem, reformatting...\r\n");
                reformat();
                pc->printf("Reformat done\r\n");
                status = fs->mount(sdbd.get());
        }
        if (debug)
                pc->printf("FileSystem Mount: \r\n%s\r\n", strerror(-status));
}

void SatFileHandler::initBlockDevice()
{
        sdbd = std::make_unique<SDBlockDevice>(hwo->mosi, hwo->miso, hwo->sclk,
                                               hwo->cs, hwo->freq, hwo->crc_on);
        int status = sdbd->init();
        if (debug) {
                pc->printf("SD Block Device Init: \r\n");
                pc->printf("%s\r\n", strerror(-status));
                pc->printf("SD Device Size: %lu\r\n", sdbd->size());
                pc->printf("SD Read Size: %lu\r\n", sdbd->get_read_size());
                pc->printf("SD Program Size: %lu\r\n",
                           sdbd->get_program_size()); // write size, basically
                pc->printf("SD Erase Size: %lu\r\n", sdbd->get_erase_size());
                ;
        }
}

void SatFileHandler::initSerial()
{
        pc = new Serial(SERIAL_TX, SERIAL_RX);
}

void SatFileHandler::reformat()
{
        int status = fs->reformat(sdbd.get());
        if (debug) {
                if (status) {
                        pc->printf("Filesystem Reformat: \r\n");
                        pc->printf("Failed\r\n");
                } else {
                        pc->printf("Filesystem Reformat: \r\n");
                        pc->printf("Success\r\n");
                }
        }
}

void SatFileHandler::mount()
{
        int status = fs->mount(sdbd.get());
        if (debug) {
                if (status) {
                        pc->printf("Filesystem Mount: \r\n");
                        pc->printf("Failed\r\n");
                } else {
                        pc->printf("Filesystem Mount: \r\n");
                        pc->printf("Success\r\n");
                }
        }
}

void SatFileHandler::unmount()
{
        int status = fs->unmount();
        if (debug) {
                if (status) {
                        pc->printf("Filesystem Unmount: \r\n");
                        pc->printf("Failed\r\n");
                } else {
                        pc->printf("Filesystem Unmount: \r\n");
                        pc->printf("Success\r\n");
                }
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
