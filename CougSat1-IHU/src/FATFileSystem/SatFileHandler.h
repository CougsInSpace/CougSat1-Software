// #include "HeapBlockDevice.h"
// #include "Mutex.h"
#include <FATFileSystem.h>
#include <SDBlockDevice.h>
#include <cstdio>
#include <fstream>
#include <mbed.h>
#include <memory>
#include <queue>
#include <string>

// using namespace std;

// class that will handle all file read/write operations to a given medium on
// the sattelite inherits from SDBlockDevice as I'm adding functionality to the
// class. though this does make the file handler limited to only an sd card over
// spi.
class SatFileHandler
{
    public:
        // create a SatFileHandler where the debug state can be enabled or
        // disabled

        SatFileHandler(PinName mosi, PinName miso, PinName sclk, PinName cs,
                       uint64_t hz = 1000000, bool crc_on = true,
                       bool debug = false);

        ~SatFileHandler();

        bool writef(std::string filenameBase, const char *message);
        bool write(std::string filenameBase, std::string &message);
        bool writeStart();

        void init();
        void reformat();
        void mount();
        void unmount();

        /*
        function to enqueue a message needed to stored on the SD card.
        input pair<fileBase, message>
        return true if enqueued; false if the queue is full

        USE FOR THREAD SAFETY
        */
        bool enqueueMessage(pair<std::string, std::string> message);

        std::string read(const std::string &fileNameFull);

    private:
        bool debug;
        bool needsReformat;
        struct HardwareOptions {
                PinName mosi;
                PinName miso;
                PinName sclk;
                PinName cs;
                uint64_t freq;
                bool crc_on;
        };

        std::unique_ptr<FATFileSystem> fs;

        std::unique_ptr<SDBlockDevice> sdbd;

        std::unique_ptr<HardwareOptions> hwo;

        // holds the current day of the sattelite will increment after each day
        uint16_t current;

        // hold the day that takes priority in removal if the device is running
        // low on storage
        uint16_t priority;

        // Serial out for testing purposes
        Serial *pc;

        bool clean(std::string dir);

        // checks for free memory
        bool check();

        void initBlockDevice();
        void initFilesystem();
        void initSerial();

        std::queue<std::pair<std::string, std::string>> inputMessages;
};
