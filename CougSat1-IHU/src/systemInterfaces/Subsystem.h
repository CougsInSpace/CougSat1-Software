/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file SubSystem.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Abstract class for subsystem and system interface template
 *
 * Contains standard methods for all subsystems and system interfaces to
 * implement
 */
#ifndef SRC_SYSTEMINTERFACES_SUBSYSTEM_H_
#define SRC_SYSTEMINTERFACES_SUBSYSTEM_H_

#include <mbed.h>

class SubSystem {
  public:

    /**
     * Deconstructor is intentionally empty
     */
    virtual ~SubSystem() {};

    /**
     * Intializes subsystem
     * @return error code
     */
    virtual uint8_t initialize() = 0;
};

#endif /* SRC_SYSTEMINTERFACES_SUBSYSTEM_H_ */
