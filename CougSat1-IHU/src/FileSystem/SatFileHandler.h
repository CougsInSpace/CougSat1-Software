#include <FATFileSystem.h>
#include <SDBlockDevice.h>
#include <fstream>
#include <mbed.h>
#include <memory>
#include <queue>
#include <string>

/// Class that will handle all file read/write operations to a given medium on
/// the sattelite. Handles all operations related to files. Currently only
/// operates on string data.
/// TODO: Update to work with other types of data.
class SatFileHandler
{
    public:
        /// Root file path.
        static const std::string rootDirectory;

        /// SPI Bus frequency: 25MHz.
        /// 25Mhz is the max frequency.
        static constexpr uint64_t frequency = 25000000;

        /// Create a SatFileHandler where the debug state can be enabled or
        /// disabled.
        /// @param mosi PinName of SPI Master in Slave out.
        /// @param miso PinName of SPI Master out Slave in.
        /// @param sclk PinName of Serial Clock for SPI.
        /// @param cs PinName of Chip Select pin on SD breakout.
        /// @param crc_on Decides whether or not to use cyclic redundancy check.
        /// @param debug Decides whether or not debug data should be outputted.

        SatFileHandler(PinName mosi, PinName miso, PinName sclk, PinName cs,
                       PinName cd, bool crc_on = true, bool debug = false);

        /// Destructor. Unmounts the filesystem and deinits the block device for
        /// a clean disconnect.
        ~SatFileHandler();

        /// Writes a string to a file
        /// @param string Name of the file.
        /// @param message C-String Message to write.
        void writef(std::string filenameBase, const char *message,
                    std::ios::openmode mode = std::ios::out);

        /// Calls writef?
        /// @param name Name of file.
        /// @param reference Reference to message.
        void write(std::string filenameBase, const std::string &message,
                   std::ios_base::openmode mode = std::ios::out);

        /// Writes whatever is in the queue.
        void writeStart();

        /// Initialize the block device, filesystem and serial connection.
        /// @return Return 0 for success, negative error code for failure.
        mbed_error_status_t init();

        /// Reformats the filesystem.
        void reformat();

        /// Mounts the filesystem.
        void mount();

        /// Unmounts the filesystem.
        void unmount();

        /// Checks for bad blocks.
        void check();

        /// Get amount of free space in bytes of the filesystem.
        /// @return Amount of free space in bytes.
        size_t freeSpace() const;

        /// Get the number of bytes in the block device.
        /// @return Size of block device in bytes.
        size_t blockDeviceSize() const;

        /*!
        \brief Function to enqueue a message to store on the SD card.

        @param message Input std::pair<fileBase, message>.

        USE FOR THREAD SAFETY
        */
        void enqueueMessage(pair<std::string, std::string> message);

        /// Read a file from the filesystem.
        /// @param fileNameFull Name of the file with extension to read.
        /// @return String of data in file.
        std::fstream read(const std::string &fileNameFull,
                          std::ios_base::openmode mode = std::ios::in);

    private:
        bool debug;
        /// Currently unused. Will be used to set if we need a reformat of
        /// filesystem.
        bool needsReformat;

        /// Contains all pin names for SPI as well as operatiing frequency and
        /// crc.
        struct HardwareOptions {
                PinName mosi;
                PinName miso;
                PinName sclk;
                PinName cs;
                PinName cd;
                bool crc_on;
        };
        /// Filesystem to be mounted.
        std::unique_ptr<FATFileSystem> fs;
        /// Block device that corresponds to the SD card.
        std::unique_ptr<SDBlockDevice> sdbd;

        std::unique_ptr<DigitalIn> cardDetect;

        /// Struct containing all the hardware options. Is deleted when init
        /// finishes.
        std::unique_ptr<HardwareOptions> hwo;

        /// Serial out for testing purposes.
        std::unique_ptr<Serial> pc;

        /// Holds the current day of the satelite will increment after each
        /// day.
        uint16_t current;

        /// Hold the day that takes priority in removal if the device is running
        /// low on storage.
        uint16_t priority;

        /// Thread for card detect light
        Thread cardThread;

        /// Queue for messages.
        std::queue<std::pair<std::string, std::string>> inputMessages;

        /// Cleans the file system...somehow.
        /// @param dir Name of directory to...clean?
        void clean(std::string dir);

        /// Creates the connection to the block device.
        /// @return Return 0 for success, negative error code for failure.
        mbed_error_status_t initBlockDevice();

        /// Mounts the file system. If the mount fails, will reformat.
        /// @return Return 0 for success, negative error code for failure.
        mbed_error_status_t initFilesystem();

        /// Create a serial connection to the host computer.
        void initSerial();

        /// Compares the values in two array. Used in check function.
        /// @param arr1 Pointer to first element of first array.
        /// @param arr2 Pointer to first element of second array.
        /// @param size Size of both arrays.
        /// Note: Function only works if arrays are same size.
        /// @return True for if they are matching, false if they are not.
        /// TODO: Update to work if sizes are different.
        bool compareArrays(uint8_t *arr1, uint8_t *arr2, size_t size);
};
