/*
 *  Message.hxx
 *  Copyright 2020 ItJustWorksTM
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef SMARTCAR_EMUL_BOIP_MESSAGE_HXX
#define SMARTCAR_EMUL_BOIP_MESSAGE_HXX

#include <utility>
#include <gsl/gsl>
#include "Address.hxx"

namespace smce::boip {

struct DataFrame {
    Address sender;
    Address receiver;
    gsl::span<const std::byte> payload;
};

struct PairingFrame {
    enum class Status : unsigned char {
        pairing_request,
        pin_request,
        pin_providal,
        pairing_reply,
    } status;
    Address sender;
    Address receiver;
    gsl::span<const char> sender_name; // only meaningful on pairing_request
    std::uint64_t pin; // only meaningful on pin_providal
};

}

#endif // SMARTCAR_EMUL_BOIP_MESSAGE_HXX
