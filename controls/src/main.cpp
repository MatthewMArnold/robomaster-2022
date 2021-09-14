/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of template-project.
 *
 * template-project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * template-project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with template-project.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef PLATFORM_HOSTED
/* hosted environment (simulator) includes --------------------------------- */
#include <iostream>

#include "tap/communication/tcp-server/tcp_server.hpp"
#include "tap/motor/motorsim/sim_handler.hpp"
#endif

#include "tap/board/board.hpp"

#include "modm/architecture/interface/delay.hpp"

/* arch includes ------------------------------------------------------------*/
#include "tap/architecture/periodic_timer.hpp"
#include "tap/architecture/profiler.hpp"

/* communication includes ---------------------------------------------------*/
#include "tap/drivers_singleton.hpp"

/* error handling includes --------------------------------------------------*/
#include "tap/errors/create_errors.hpp"

/* control includes ---------------------------------------------------------*/
#include "tap/architecture/clock.hpp"

using tap::Drivers;

/* define timers here -------------------------------------------------------*/
tap::arch::PeriodicMilliTimer sendMotorTimeout(2);

// Place any sort of input/output initialization here. For example, place
// serial init stuff here.
static void initializeIo(tap::Drivers *drivers);

// Anything that you would like to be called place here. It will be called
// very frequently. Use PeriodicMilliTimers if you don't want something to be
// called as frequently.
static void updateIo(tap::Drivers *drivers);

int main()
{
#ifdef PLATFORM_HOSTED
    std::cout << "Simulation starting..." << std::endl;
#endif

    /*
     * NOTE: We are using DoNotUse_getDrivers here because in the main
     *      robot loop we must access the singleton drivers to update
     *      IO states and run the scheduler.
     */
    tap::Drivers *drivers = tap::DoNotUse_getDrivers();

    Board::initialize();
    initializeIo(drivers);

#ifdef PLATFORM_HOSTED
    tap::motorsim::SimHandler::resetMotorSims();
    // Blocking call, waits until Windows Simulator connects.
    tap::communication::TCPServer::MainServer()->getConnection();
#endif

    // tap::motor::DjiMotor motor1(drivers, tap::motor::MOTOR1, tap::can::CanBus::CAN_BUS1, 0, "name1");
    // motor1.setDesiredOutput(10);

    drivers->pwm.write(.5f, tap::gpio::Pwm::C1);

    tap::motor::DjiMotor motor(drivers, tap::motor::MOTOR3, tap::can::CanBus::CAN_BUS1, false, "cool motor");
    motor.initialize();
    motor.setDesiredOutput(5000);

    while (1)
    {   
        if (sendMotorTimeout.execute())
        {
            drivers->djiMotorTxHandler.processCanSendData();
        }
        drivers->canRxHandler.pollCanData();
        modm::delay_us(10);
        // drivers->leds.set(tap::gpio::Leds::Red, true);
        // drivers->leds.set(tap::gpio::Leds::Green, true);
        // do this as fast as you can
        // PROFILE(drivers->profiler, updateIo, (drivers));

        // if (sendMotorTimeout.execute())
        // {
        //     PROFILE(drivers->profiler, drivers->commandScheduler.run, ());
        //     PROFILE(drivers->profiler, drivers->djiMotorTxHandler.processCanSendData, ());
        //     PROFILE(drivers->profiler, drivers->terminalSerial.update, ());
        // }
        // modm::delay_us(100);
        // modm::delay_ms(500);
        // drivers->leds.set(tap::gpio::Leds::Red, false);
        // drivers->leds.set(tap::gpio::Leds::Green, false);
        // modm::delay_ms(500);
    }
    return 0;
}

static void initializeIo(tap::Drivers *drivers)
{
    drivers->analog.init();
    drivers->pwm.init();
    drivers->digital.init();
    drivers->leds.init();
    drivers->can.initialize();
    drivers->errorController.init();
    drivers->remote.initialize();
    drivers->refSerial.initialize();
    drivers->terminalSerial.initialize();
    drivers->schedulerTerminalHandler.init();
    drivers->djiMotorTerminalSerialHandler.init();
}

static void updateIo(tap::Drivers *drivers)
{
#ifdef PLATFORM_HOSTED
    tap::motorsim::SimHandler::updateSims();
#endif

    drivers->canRxHandler.pollCanData();
    drivers->refSerial.updateSerial();
    drivers->remote.read();
}
