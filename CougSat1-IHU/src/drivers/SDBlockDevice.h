/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Introduction
 * ------------
 * SD and MMC cards support a number of interfaces, but common to them all
 * is one based on SPI. Since we already have the mbed SPI Interface, it will
 * be used for SD cards.
 *
 * The main reference I'm using is Chapter 7, "SPI Mode" of:
 *  http://www.sdcard.org/developers/tech/sdcard/pls/Simplified_Physical_Layer_Spec.pdf
 *
 * SPI Startup
 * -----------
 * The SD card powers up in SD mode. The start-up procedure is complicated
 * by the requirement to support older SDCards in a backwards compatible
 * way with the new higher capacity variants SDHC and SDHC.
 *
 * The following figures from the specification with associated text describe
 * the SPI mode initialisation process:
 *  - Figure 7-1: SD Memory Card State Diagram (SPI mode)
 *  - Figure 7-2: SPI Mode Initialization Flow
 *
 * Firstly, a low initial clock should be selected (in the range of 100-
 * 400kHZ). After initialisation has been completed, the switch to a
 * higher clock speed can be made (e.g. 1MHz). Newer cards will support
 * higher speeds than the default _transfer_sck defined here.
 *
 * Next, note the following from the SDCard specification (note to
 * Figure 7-1):
 *
 *  In any of the cases CMD1 is not recommended because it may be difficult for the host
 *  to distinguish between MultiMediaCard and SD Memory Card
 *
 * Hence CMD1 is not used for the initialisation sequence.
 *
 * The SPI interface mode is selected by asserting CS low and sending the
 * reset command (CMD0). The card will respond with a (R1) response.
 * In practice many cards initially respond with 0xff or invalid data
 * which is ignored. Data is read until a valid response is received
 * or the number of re-reads has exceeded a maximim count. If a valid
 * response is not received then the CMD0 can be retried. This
 * has been found to successfully initialise cards where the SPI master
 * (on MCU) has been reset but the SDCard has not, so the first
 * CMD0 may be lost.
 *
 * CMD8 is optionally sent to determine the voltage range supported, and
 * indirectly determine whether it is a version 1.x SD/non-SD card or
 * version 2.x. I'll just ignore this for now.
 *
 * ACMD41 is repeatedly issued to initialise the card, until "in idle"
 * (bit 0) of the R1 response goes to '0', indicating it is initialised.
 *
 * You should also indicate whether the host supports High Capicity cards,
 * and check whether the card is high capacity - i'll also ignore this
 *
 * SPI Protocol
 * ------------
 * The SD SPI protocol is based on transactions made up of 8-bit words, with
 * the host starting every bus transaction by asserting the CS signal low. The
 * card always responds to commands, data blocks and errors.
 *
 * The protocol supports a CRC, but by default it is off (except for the
 * first reset CMD0, where the CRC can just be pre-calculated, and CMD8)
 * I'll leave the CRC off I think!
 *
 * Standard capacity cards have variable data block sizes, whereas High
 * Capacity cards fix the size of data block to 512 bytes. I'll therefore
 * just always use the Standard Capacity cards with a block size of 512 bytes.
 * This is set with CMD16.
 *
 * You can read and write single blocks (CMD17, CMD25) or multiple blocks
 * (CMD18, CMD25). For simplicity, I'll just use single block accesses. When
 * the card gets a read command, it responds with a response token, and then
 * a data token or an error.
 *
 * SPI Command Format
 * ------------------
 * Commands are 6-bytes long, containing the command, 32-bit argument, and CRC.
 *
 * +---------------+------------+------------+-----------+----------+--------------+
 * | 01 | cmd[5:0] | arg[31:24] | arg[23:16] | arg[15:8] | arg[7:0] | crc[6:0] | 1 |
 * +---------------+------------+------------+-----------+----------+--------------+
 *
 * As I'm not using CRC, I can fix that byte to what is needed for CMD0 (0x95)
 *
 * All Application Specific commands shall be preceded with APP_CMD (CMD55).
 *
 * SPI Response Format
 * -------------------
 * The main response format (R1) is a status byte (normally zero). Key flags:
 *  idle - 1 if the card is in an idle state/initialising
 *  cmd  - 1 if an illegal command code was detected
 *
 *    +-------------------------------------------------+
 * R1 | 0 | arg | addr | seq | crc | cmd | erase | idle |
 *    +-------------------------------------------------+
 *
 * R1b is the same, except it is followed by a busy signal (zeros) until
 * the first non-zero byte when it is ready again.
 *
 * Data Response Token
 * -------------------
 * Every data block written to the card is acknowledged by a byte
 * response token
 *
 * +----------------------+
 * | xxx | 0 | status | 1 |
 * +----------------------+
 *              010 - OK!
 *              101 - CRC Error
 *              110 - Write Error
 *
 * Single Block Read and Write
 * ---------------------------
 *
 * Block transfers have a byte header, followed by the data, followed
 * by a 16-bit CRC. In our case, the data will always be 512 bytes.
 *
 * +------+---------+---------+- -  - -+---------+-----------+----------+
 * | 0xFE | data[0] | data[1] |        | data[n] | crc[15:8] | crc[7:0] |
 * +------+---------+---------+- -  - -+---------+-----------+----------+
 */

#ifndef SRC_DRIVERS_SDBLOCKDEVICE_H_
#define SRC_DRIVERS_SDBLOCKDEVICE_H_

/* If the target has no SPI support then SDCard is not supported */
#ifdef DEVICE_SPI

#include "BlockDevice.h"
#include "mbed.h"
#include "platform/PlatformMutex.h"

/** Access an SD Card using SPI
 *
 * @code
 * #include "mbed.h"
 * #include "SDBlockDevice.h"
 *
 * SDBlockDevice sd(p5, p6, p7, p12); // mosi, miso, sclk, cs
 * uint8_t block[512] = "Hello World!\n";
 *
 * int main() {
 *     sd.init();
 *     sd.write(block, 0, 512);
 *     sd.read(block, 0, 512);
 *     printf("%s", block);
 *     sd.deinit();
 * }
 */
class SDBlockDevice : public BlockDevice {
public:
    /** Lifetime of an SD card
     */
    SDBlockDevice(PinName mosi, PinName miso, PinName sclk, PinName cs, uint64_t hz=1000000);
    virtual ~SDBlockDevice();

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int deinit();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);

    /** Mark blocks as no longer in use
     *
     *  This function provides a hint to the underlying block device that a region of blocks
     *  is no longer in use and may be erased without side effects. Erase must still be called
     *  before programming, but trimming allows flash-translation-layers to schedule erases when
     *  the device is not busy.
     *
     *  @param addr     Address of block to mark as unused
     *  @param size     Size to mark as unused in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int trim(bd_addr_t addr, bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual bd_size_t get_program_size() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size() const;

    /** Set the transfer frequency
     *
     *  @param         Transfer frequency
     *  @note Max frequency supported is 25MHZ
     */
    virtual int frequency(uint64_t freq);


private:
    /* Commands : Listed below are commands supported
     * in SPI mode for SD card : Only Mandatory ones
     */
    enum cmdSupported {
        CMD_NOT_SUPPORTED = -1,             /**< Command not supported error */
        CMD0_GO_IDLE_STATE = 0,             /**< Resets the SD Memory Card */
        CMD1_SEND_OP_COND = 1,              /**< Sends host capacity support */
        CMD6_SWITCH_FUNC = 6,               /**< Check and Switches card function */
        CMD8_SEND_IF_COND = 8,              /**< Supply voltage info */
        CMD9_SEND_CSD = 9,                  /**< Provides Card Specific data */
        CMD10_SEND_CID = 10,                /**< Provides Card Identification */
        CMD12_STOP_TRANSMISSION = 12,       /**< Forces the card to stop transmission */
        CMD13_SEND_STATUS = 13,             /**< Card responds with status */
        CMD16_SET_BLOCKLEN = 16,            /**< Length for SC card is set */
        CMD17_READ_SINGLE_BLOCK = 17,       /**< Read single block of data */
        CMD18_READ_MULTIPLE_BLOCK = 18,     /**< Card transfers data blocks to host until interrupted
                                                 by a STOP_TRANSMISSION command */
        CMD24_WRITE_BLOCK = 24,             /**< Write single block of data */
        CMD25_WRITE_MULTIPLE_BLOCK = 25,    /**< Continuously writes blocks of data until
                                                 'Stop Tran' token is sent */
        CMD27_PROGRAM_CSD = 27,             /**< Programming bits of CSD */
        CMD32_ERASE_WR_BLK_START_ADDR = 32, /**< Sets the address of the first write
                                                 block to be erased. */
        CMD33_ERASE_WR_BLK_END_ADDR = 33,   /**< Sets the address of the last write
                                                 block of the continuous range to be erased.*/
        CMD38_ERASE = 38,                   /**< Erases all previously selected write blocks */
        CMD55_APP_CMD = 55,                 /**< Extend to Applications specific commands */
        CMD56_GEN_CMD = 56,                 /**< General Purpose Command */
        CMD58_READ_OCR = 58,                /**< Read OCR register of card */
        CMD59_CRC_ON_OFF = 59,              /**< Turns the CRC option on or off*/
        // App Commands
        ACMD6_SET_BUS_WIDTH = 6,
        ACMD13_SD_STATUS = 13,
        ACMD22_SEND_NUM_WR_BLOCKS = 22,
        ACMD23_SET_WR_BLK_ERASE_COUNT = 23,
        ACMD41_SD_SEND_OP_COND = 41,
        ACMD42_SET_CLR_CARD_DETECT = 42,
        ACMD51_SEND_SCR = 51,
    };

    uint8_t _card_type;
    int _cmd(SDBlockDevice::cmdSupported cmd, uint32_t arg, bool isAcmd=0, uint32_t *resp=NULL);
    int _cmd8();

    /*  Move the SDCard into the SPI Mode idle state
     *
     *  The card is transitioned from SDCard mode to SPI mode by sending the
     *  CMD0 (GO_IDLE_STATE) command with CS asserted. See the notes in the
     *  "SPI Startup" section of the comments at the head of the
     *  implementation file for further details and specification references.
     *
     *  @return         Response form the card. R1_IDLE_STATE (0x1), the successful
     *                  response from CMD0. R1_XXX_XXX for more response
     */
    uint32_t _go_idle_state();
    int _initialise_card();

    bd_size_t _sectors;
    bd_size_t _sd_sectors();

    bool _is_valid_trim(bd_addr_t addr, bd_size_t size);

    /* SPI functions */
    Timer _spi_timer;               /**< Timer Class object used for busy wait */
    uint32_t _init_sck;             /**< Intial SPI frequency */
    uint32_t _transfer_sck;         /**< SPI frequency during data transfer/after initialization */
    SPI _spi;                       /**< SPI Class object */

    /* SPI initialization function */
    void _spi_init();
    uint8_t _cmd_spi(SDBlockDevice::cmdSupported cmd, uint32_t arg);
    void _spi_wait(uint8_t count);

    bool _wait_token(uint8_t token);        /**< Wait for token */
    bool _wait_ready(uint16_t ms=300);      /**< 300ms default wait for card to be ready */
    int _read(uint8_t * buffer, uint32_t length);
    int _read_bytes(uint8_t * buffer, uint32_t length);
    uint8_t _write(const uint8_t *buffer,uint8_t token, uint32_t length);
    int _freq(void);

    /* Chip Select and SPI mode select */
    DigitalOut _cs;
    void _select();
    void _deselect();

    virtual void lock() {
        _mutex.lock();
    }

    virtual void unlock() {
        _mutex.unlock();
    }

    PlatformMutex _mutex;
    bd_size_t _block_size;
    bd_size_t _erase_size;
    bool _is_initialized;
    bool _dbg;
};

#endif  /* DEVICE_SPI */
#endif /* SRC_DRIVERS_SDBLOCKDEVICE_H_ */
