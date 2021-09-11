/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of Taproot.
 *
 * Taproot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Taproot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Taproot.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "analog.hpp"

#include "tap/board/board.hpp"

using namespace Board;

namespace tap
{
namespace gpio
{
void Analog::init()
{
#ifndef PLATFORM_HOSTED

#endif
}

uint16_t Analog::read(Pin pin) const
{
#ifdef PLATFORM_HOSTED
    return 0;
#else
    switch (pin)
    {
        default:
            return 0;
    }
#endif
}
}  // namespace gpio

}  // namespace tap
