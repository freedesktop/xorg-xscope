/*
 * Copyright © 2001 Keith Packard, member of The XFree86 Project, Inc.
 * Copyright (c) 2012, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
#define _RANDR_SERVER_
#include "scope.h"
#include "x11.h"
#include "randrscope.h"
#include "extensions.h"

static unsigned char RANDRRequest, RANDRError, RANDREvent;

#define RANDRNError 4 /* as of RandR 1.4 - 2012-07-03 */

static void
randr_decode_req(FD fd, const unsigned char *buf)
{
    short Major = IByte(&buf[0]);
    short Minor = IByte(&buf[1]);

    switch (Minor) {
/* RandR 0.x & 1.0 requests are intermixed in minor codes 0-5 */
    case 0:
        RandrQueryVersion(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 1:
        RandrOldGetScreenInfo(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 2:
        RandrSetScreenConfig(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 3:
        RandrOldScreenChangeSelectInput(fd, buf);
        break;
    case 4:
        RandrSelectInput(fd, buf);
        break;
    case 5:
        RandrGetScreenInfo(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;

/* RandR 1.2 additions */
    case 6:
        RandrGetScreenSizeRange(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 7:
        RandrSetScreenSize(fd, buf);
        break;
    case 8:
        RandrGetScreenResources(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 9:
        RandrGetOutputInfo(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 10:
        RandrListOutputProperties(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 11:
        RandrQueryOutputProperty(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 12:
        RandrConfigureOutputProperty(fd, buf);
        break;
    case 13:
        RandrChangeOutputProperty(fd, buf);
        break;
    case 14:
        RandrDeleteOutputProperty(fd, buf);
        break;
    case 15:
        RandrGetOutputProperty(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 16:
        RandrCreateMode(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 17:
        RandrDestroyMode(fd, buf);
        break;
    case 18:
        RandrAddOutputMode(fd, buf);
        break;
    case 19:
        RandrDeleteOutputMode(fd, buf);
        break;
    case 20:
        RandrGetCrtcInfo(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 21:
        RandrSetCrtcConfig(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 22:
        RandrGetCrtcGammaSize(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 23:
        RandrGetCrtcGamma(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 24:
        RandrSetCrtcGamma(fd, buf);
        break;

/* RandR 1.3 additions */
    case 25:
        RandrGetScreenResourcesCurrent(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 26:
        RandrSetCrtcTransform(fd, buf);
        break;
    case 27:
        RandrGetCrtcTransform(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 28:
        RandrGetPanning(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 29:
        RandrSetPanning(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 30:
        RandrSetOutputPrimary(fd, buf);
        break;
    case 31:
        RandrGetOutputPrimary(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;

/* RandR 1.4 additions */
    case 32:
        RandrGetProviders(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 33:
        RandrGetProviderInfo(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 34:
        RandrSetProviderOffloadSink(fd, buf);
        break;
    case 35:
        RandrSetProviderOutputSource(fd, buf);
        break;
    case 36:
        RandrListProviderProperties(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 37:
        RandrQueryProviderProperty(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    case 38:
        RandrConfigureProviderProperty(fd, buf);
        break;
    case 39:
        RandrChangeProviderProperty(fd, buf);
        break;
    case 40:
        RandrDeleteProviderProperty(fd, buf);
        break;
    case 41:
        RandrGetProviderProperty(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;

    default:
        ExtendedRequest(fd, buf);
        ExtendedReplyExpected(fd, Major, Minor);
        break;
    }
}

static void
randr_decode_reply(FD fd, const unsigned char *buf, short RequestMinor)
{
    switch (RequestMinor) {
/* RandR 0.x & 1.0 requests are intermixed in minor codes 0-5 */
    case 0:
        RandrQueryVersionReply(fd, buf);
        break;
    case 1:
        RandrOldGetScreenInfoReply(fd, buf);
        break;
    case 2:
        RandrSetScreenConfigReply(fd, buf);
        break;
    case 5:
        RandrGetScreenInfoReply(fd, buf);
        break;

/* RandR 1.2 additions */
    case 6:
        RandrGetScreenSizeRangeReply(fd, buf);
        break;
    case 8:
        RandrGetScreenResourcesReply(fd, buf);
        break;
    case 9:
        RandrGetOutputInfoReply(fd, buf);
        break;
    case 10:
        RandrListOutputPropertiesReply(fd, buf);
        break;
    case 11:
        RandrQueryOutputPropertyReply(fd, buf);
        break;
    case 15:
        RandrGetOutputPropertyReply(fd, buf);
        break;
    case 16:
        RandrCreateModeReply(fd, buf);
        break;
    case 20:
        RandrGetCrtcInfoReply(fd, buf);
        break;
    case 21:
        RandrSetCrtcConfigReply(fd, buf);
        break;
    case 22:
        RandrGetCrtcGammaSizeReply(fd, buf);
        break;
    case 23:
        RandrGetCrtcGammaReply(fd, buf);
        break;

/* RandR 1.3 additions */
    case 25:
        RandrGetScreenResourcesCurrentReply(fd, buf);
        break;
    case 27:
        RandrGetCrtcTransformReply(fd, buf);
        break;
    case 28:
        RandrGetPanningReply(fd, buf);
        break;
    case 29:
        RandrSetPanningReply(fd, buf);
        break;
    case 31:
        RandrGetOutputPrimaryReply(fd, buf);
        break;

/* RandR 1.4 additions */
    case 32:
        RandrGetProvidersReply(fd, buf);
        break;
    case 33:
        RandrGetProviderInfoReply(fd, buf);
        break;
    case 36:
        RandrListProviderPropertiesReply(fd, buf);
        break;
    case 37:
        RandrQueryProviderPropertyReply(fd, buf);
        break;
    case 41:
        RandrGetProviderPropertyReply(fd, buf);
        break;

    default:
        UnknownReply(buf);
        break;
    }
}

static void
randr_decode_event(FD fd, const unsigned char *buf)
{
    short event = IByte(&buf[0]) - RANDREvent;

    switch (event) {
    case 0:
        RandrScreenChangeNotifyEvent(buf);
        break;
/* RandR 1.2 and later additions share this event code and use sub-codes */
    case 1:
        {
            short minor = IByte(&buf[1]);
            switch (minor) {
            case 0:
                RandrCrtcChangeNotifyEvent(buf);
                break;
            case 1:
                RandrOutputChangeNotifyEvent(buf);
                break;
            case 2:
                RandrOutputPropertyNotifyEvent(buf);
                break;
            case 3:
                RandrProviderChangeNotifyEvent(buf);
                break;
            case 4:
                RandrProviderPropertyNotifyEvent(buf);
                break;
            case 5:
                RandrResourceChangeNotifyEvent(buf);
                break;
            default:
                UnknownEvent(buf);
                break;
            }
        }
        break;
    default:
        UnknownEvent(buf);
        break;
    }
}

static void
randr_decode_error(FD fd, const unsigned char *buf)
{
    short error = IByte(&buf[1]) - RANDRError;

    switch (error) {
/* RandR 1.2 additions */
    case 0:
        RandrOutputError(fd, buf);
        break;
    case 1:
        RandrCrtcError(fd, buf);
        break;
    case 2:
        RandrModeError(fd, buf);
        break;
/* RandR 1.4 additions */
    case 3:
        RandrProviderError(fd, buf);
        break;
    default:
        UnknownError(buf);
        break;
    }
}

void
InitializeRANDR(const unsigned char *buf)
{
    TYPE p;

    RANDRRequest = (unsigned char) (buf[9]);
    RANDREvent = (unsigned char) (buf[10]);
    RANDRError = (unsigned char) (buf[11]);

    DefineEValue(&TD[REQUEST], (unsigned long) RANDRRequest, "RandrRequest");
    DefineEValue(&TD[REPLY], (unsigned long) RANDRRequest, "RandrReply");
    DefineEValue(&TD[EVENT], (unsigned long) RANDREvent,
                 "RRScreenChangeNotify");
    DefineEValue(&TD[EVENT], (unsigned long) RANDREvent + 1, "RandrNotify");

    DefineEValue(&TD[ERROR], (unsigned long) RANDRError + 0, "BadOutput");
    DefineEValue(&TD[ERROR], (unsigned long) RANDRError + 1, "BadCrtc");
    DefineEValue(&TD[ERROR], (unsigned long) RANDRError + 2, "BadMode");
    DefineEValue(&TD[ERROR], (unsigned long) RANDRError + 3, "BadProvider");


    p = DefineType(RANDRREQUEST, ENUMERATED, "RANDRREQUEST",
                   (PrintProcType) PrintENUMERATED);
    DefineEValue(p, 0L, "RandrQueryVersion");
    DefineEValue(p, 1L, "RandrOldGetScreenInfo");
    DefineEValue(p, 2L, "RandrSetScreenConfig");
    DefineEValue(p, 3L, "RandrOldScreenChangeSelectInput");
    DefineEValue(p, 4L, "RandrSelectInput");
    DefineEValue(p, 5L, "RandrGetScreenInfo");
    DefineEValue(p, 6L, "RandrGetScreenSizeRange");
    DefineEValue(p, 7L, "RandrSetScreenSize");
    DefineEValue(p, 8L, "RandrGetScreenResources");
    DefineEValue(p, 9L, "RandrGetOutputInfo");
    DefineEValue(p, 10L, "RandrListOutputProperties");
    DefineEValue(p, 11L, "RandrQueryOutputProperty");
    DefineEValue(p, 12L, "RandrConfigureOutputProperty");
    DefineEValue(p, 13L, "RandrChangeOutputProperty");
    DefineEValue(p, 14L, "RandrDeleteOutputProperty");
    DefineEValue(p, 15L, "RandrGetOutputProperty");
    DefineEValue(p, 16L, "RandrCreateMode");
    DefineEValue(p, 17L, "RandrDestroyMode");
    DefineEValue(p, 18L, "RandrAddOutputMode");
    DefineEValue(p, 19L, "RandrDeleteOutputMode");
    DefineEValue(p, 20L, "RandrGetCrtcInfo");
    DefineEValue(p, 21L, "RandrSetCrtcConfig");
    DefineEValue(p, 22L, "RandrGetCrtcGammaSize");
    DefineEValue(p, 23L, "RandrGetCrtcGamma");
    DefineEValue(p, 24L, "RandrSetCrtcGamma");
    DefineEValue(p, 25L, "RandrGetScreenResourcesCurrent");
    DefineEValue(p, 26L, "RandrSetCrtcTransform");
    DefineEValue(p, 27L, "RandrGetCrtcTransform");
    DefineEValue(p, 28L, "RandrGetPanning");
    DefineEValue(p, 29L, "RandrSetPanning");
    DefineEValue(p, 30L, "RandrSetOutputPrimary");
    DefineEValue(p, 31L, "RandrGetOutputPrimary");
    DefineEValue(p, 32L, "RandrGetProviders");
    DefineEValue(p, 33L, "RandrGetProviderInfo");
    DefineEValue(p, 34L, "RandrSetProviderOffloadSink");
    DefineEValue(p, 35L, "RandrSetProviderOutputSource");
    DefineEValue(p, 36L, "RandrListProviderProperties");
    DefineEValue(p, 37L, "RandrQueryProviderProperty");
    DefineEValue(p, 38L, "RandrConfigureProviderProperty");
    DefineEValue(p, 39L, "RandrChangeProviderProperty");
    DefineEValue(p, 40L, "RandrDeleteProviderProperty");
    DefineEValue(p, 41L, "RandrGetProviderProperty");

    p = DefineType(RANDRREPLY, ENUMERATED, "RANDRREPLY",
                   (PrintProcType) PrintENUMERATED);
    DefineEValue(p, 0L, "QueryVersion");
    DefineEValue(p, 1L, "OldGetScreenInfo");
    DefineEValue(p, 2L, "SetScreenConfig");
    DefineEValue(p, 3L, "OldScreenChangeSelectInput");
    DefineEValue(p, 4L, "SelectInput");
    DefineEValue(p, 5L, "GetScreenInfo");
    DefineEValue(p, 6L, "GetScreenSizeRange");
    DefineEValue(p, 7L, "SetScreenSize");
    DefineEValue(p, 8L, "GetScreenResources");
    DefineEValue(p, 9L, "GetOutputInfo");
    DefineEValue(p, 10L, "ListOutputProperties");
    DefineEValue(p, 11L, "QueryOutputProperty");
    DefineEValue(p, 12L, "ConfigureOutputProperty");
    DefineEValue(p, 13L, "ChangeOutputProperty");
    DefineEValue(p, 14L, "DeleteOutputProperty");
    DefineEValue(p, 15L, "GetOutputProperty");
    DefineEValue(p, 16L, "CreateMode");
    DefineEValue(p, 17L, "DestroyMode");
    DefineEValue(p, 18L, "AddOutputMode");
    DefineEValue(p, 19L, "DeleteOutputMode");
    DefineEValue(p, 20L, "GetCrtcInfo");
    DefineEValue(p, 21L, "SetCrtcConfig");
    DefineEValue(p, 22L, "GetCrtcGammaSize");
    DefineEValue(p, 23L, "GetCrtcGamma");
    DefineEValue(p, 24L, "SetCrtcGamma");
    DefineEValue(p, 25L, "GetScreenResourcesCurrent");
    DefineEValue(p, 26L, "SetCrtcTransform");
    DefineEValue(p, 27L, "GetCrtcTransform");
    DefineEValue(p, 28L, "GetPanning");
    DefineEValue(p, 29L, "SetPanning");
    DefineEValue(p, 30L, "SetOutputPrimary");
    DefineEValue(p, 31L, "GetOutputPrimary");
    DefineEValue(p, 32L, "GetProviders");
    DefineEValue(p, 33L, "GetProviderInfo");
    DefineEValue(p, 34L, "SetProviderOffloadSink");
    DefineEValue(p, 35L, "SetProviderOutputSource");
    DefineEValue(p, 36L, "ListProviderProperties");
    DefineEValue(p, 37L, "QueryProviderProperty");
    DefineEValue(p, 38L, "ConfigureProviderProperty");
    DefineEValue(p, 39L, "ChangeProviderProperty");
    DefineEValue(p, 40L, "DeleteProviderProperty");
    DefineEValue(p, 41L, "GetProviderProperty");

    p = DefineType(RANDREVENT, ENUMERATED, "RANDREVENT",
                   (PrintProcType) PrintENUMERATED);
    /* V1.2 additions */
    DefineEValue(p, 0L, "RRCrtcChangeNotify");
    DefineEValue(p, 1L, "RROutputChangeNotify");
    DefineEValue(p, 2L, "RROutputPropertyNotify");
    /* V1.4 additions */
    DefineEValue(p, 3L, "RRProviderChangeNotify");
    DefineEValue(p, 4L, "RRProviderPropertyNotify");
    DefineEValue(p, 5L, "RRResourceChangeNotify");

    p = DefineType(SETofRREVENT, SET, "SETofRREVENT", (PrintProcType) PrintSET);
    /* Event selection bits */
    DefineEValue(p, (1L << 0), "ScreenChangeNotifyMask");
    /* V1.2 additions */
    DefineEValue(p, (1L << 1), "CrtcChangeNotifyMask");
    DefineEValue(p, (1L << 2), "OutputChangeNotifyMask");
    DefineEValue(p, (1L << 3), "OutputPropertyNotifyMask");
    /* V1.4 additions */
    DefineEValue(p, (1L << 4), "ProviderChangeNotifyMask");
    DefineEValue(p, (1L << 5), "ProviderPropertyNotifyMask");
    DefineEValue(p, (1L << 6), "ResourceChangeNotifyMask");

    p = DefineType(SETofROTATION, SET, "ROTATION", (PrintProcType) PrintSET);
    DefineEValue(p, 1L, "Rotate_0");
    DefineEValue(p, 2L, "Rotate_90");
    DefineEValue(p, 4L, "Rotate_180");
    DefineEValue(p, 8L, "Rotate_270");
    DefineEValue(p, 16L, "Reflect_X");
    DefineEValue(p, 32L, "Reflect_Y");

    DefineType(SCREENSIZE, RECORD, "SCREENSIZE", PrintSCREENSIZE);

    /* RandR 1.2 additions */
    p = DefineType(SETofMODEFLAG, SET, "MODEFLAG", (PrintProcType) PrintSET);
    DefineEValue(p, 0x00000001, "HSyncPositive");
    DefineEValue(p, 0x00000002, "HSyncNegative");
    DefineEValue(p, 0x00000004, "VSyncPositive");
    DefineEValue(p, 0x00000008, "VSyncNegative");
    DefineEValue(p, 0x00000010, "Interlace");
    DefineEValue(p, 0x00000020, "DoubleScan");
    DefineEValue(p, 0x00000040, "CSync");
    DefineEValue(p, 0x000100080, "CSyncPositive");
    DefineEValue(p, 0x00000100, "CSyncNegative");
    DefineEValue(p, 0x00000200, "HSkewPresent");
    DefineEValue(p, 0x00000400, "BCast");
    DefineEValue(p, 0x00000800, "PixelMultiplex");
    DefineEValue(p, 0x00001000, "DoubleClock");
    DefineEValue(p, 0x00002000, "ClockDivideBy2");

    DefineType(MODEINFO, RECORD, "MODEINFO", PrintMODEINFO);
    DefineType(MODE, BUILTIN, "MODE", PrintMODE);
    DefineType(CRTC, BUILTIN, "CRTC", PrintCRTC);
    DefineType(OUTPUT, BUILTIN, "OUTPUT", PrintOUTPUT);

    p = DefineType(CONNECTION, ENUMERATED, "CONNECTION",
                   (PrintProcType) PrintENUMERATED);
    DefineEValue(p, 0L, "Connected");
    DefineEValue(p, 1L, "Disconnected");
    DefineEValue(p, 2L, "UnknownConnection");

    /* RandR depends on Render's SUBPIXEL & TRANSFORM types */
    InitializeCommonRenderTypes();

    /* RandR 1.4 additions */
    p = DefineType(SETofPROVIDER_CAPS, SET, "PROVIDER_CAPS",
                   (PrintProcType) PrintSET);
    DefineEValue(p, 0x00000001, "SourceOutput");
    DefineEValue(p, 0x00000002, "SinkOutput");
    DefineEValue(p, 0x00000004, "SourceOffload");
    DefineEValue(p, 0x00000008, "SinkOffload");


    InitializeExtensionDecoder(RANDRRequest, randr_decode_req,
                               randr_decode_reply);
    InitializeExtensionEventDecoder(RANDREvent, randr_decode_event);
    InitializeExtensionEventDecoder(RANDREvent + 1, randr_decode_event);
    for (int errcode = RANDRError; errcode < (RANDRError + RANDRNError);
         errcode++) {
        InitializeExtensionErrorDecoder(errcode, randr_decode_error);
    }
}
