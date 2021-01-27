#ifndef SRC_SUBSYSTEMS_IFJR_H_
#define SRC_SUBSYSTEMS_IFJR_H_

#include "Subsystem.h"
#include <mbed.h>

/// Singleton class to manage IFJR.
/// use IFJR::getInstance() to access it.
class IFJR : public Subsystem
{
    public:
        /// Retrieves the static instance of the IFJR object
        /// @return IFJR object reference.
        static IFJR &getInstance();

        /// Destroy the IFJR::IFJR object.
        ~IFJR();

        /// Initialize the IFJR.
        /// @return mbed_error_status_t.
        mbed_error_status_t initialize();

        void setI2C(I2C &i2c);

    private:
        Serial *pc;
        IFJR()
        {
                pc = new Serial(SERIAL_TX, SERIAL_RX);
        }
        IFJR(IFJR &&) = delete;
        IFJR &operator=(IFJR &&) = delete;
        IFJR(const IFJR &) = delete;
        IFJR &operator=(const IFJR &) = delete;

        I2C *i2c;
};

#endif /* SRC_SUBSYSTEMS_IFJR_H_ */