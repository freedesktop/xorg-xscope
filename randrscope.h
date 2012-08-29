#ifndef _RANDRSCOPE_H_
#define _RANDRSCOPE_H_

#define RANDRREQUESTHEADER  "RANDRREQUEST"
#define RANDRREPLYHEADER    "RANDRREPLY"
#define RANDREVENTHEADER    "RANDREVENT"

/*
  Aliases for types for RandR to x11.h types - used for types we don't
  have specialized printing routines for now, but may want to someday.
*/
#define PROVIDER                CARD32          /* XID */

/*
  To aid in making the choice between level 1 and level 2, we
  define the following define, which does not print relatively
  unimportant fields.
*/

#define printfield(a,b,c,d,e) if (Verbose > 1) PrintField(a,b,c,d,e)

extern void RandrQueryVersion(FD fd, const unsigned char *buf);
extern void RandrQueryVersionReply(FD fd, const unsigned char *buf);

/* Randr 0.x - not used in Randr 1.x */
extern void RandrOldGetScreenInfo(FD fd, const unsigned char *buf);
extern void RandrOldGetScreenInfoReply(FD fd, const unsigned char *buf);
extern void RandrOldScreenChangeSelectInput(FD fd, const unsigned char *buf);
extern void RandrOldSetScreenConfig(FD fd, const unsigned char *buf);
extern void RandrOldSetScreenConfigReply(FD fd, const unsigned char *buf);

extern void RandrOldScreenChangeNotifyEvent(const unsigned char *buf);

/* Randr 1.0 */
extern int PrintSCREENSIZE(const unsigned char *buf);

extern void RandrSetScreenConfig(FD fd, const unsigned char *buf);
extern void RandrSetScreenConfigReply(FD fd, const unsigned char *buf);
extern void RandrSelectInput(FD fd, const unsigned char *buf);
extern void RandrGetScreenInfo(FD fd, const unsigned char *buf);
extern void RandrGetScreenInfoReply(FD fd, const unsigned char *buf);

extern void RandrScreenChangeNotifyEvent(const unsigned char *buf);

/* Randr 1.2 additions */
extern int PrintMODE(const unsigned char *buf);
extern int PrintMODEINFO(const unsigned char *buf);
extern int PrintCRTC(const unsigned char *buf);
extern int PrintOUTPUT(const unsigned char *buf);

extern void RandrGetScreenSizeRange(FD fd, const unsigned char *buf);
extern void RandrGetScreenSizeRangeReply(FD fd, const unsigned char *buf);
extern void RandrSetScreenSize(FD fd, const unsigned char *buf);
extern void RandrGetScreenResources(FD fd, const unsigned char *buf);
extern void RandrGetScreenResourcesReply(FD fd, const unsigned char *buf);
extern void RandrGetOutputInfo(FD fd, const unsigned char *buf);
extern void RandrGetOutputInfoReply(FD fd, const unsigned char *buf);
extern void RandrListOutputProperties(FD fd, const unsigned char *buf);
extern void RandrListOutputPropertiesReply(FD fd, const unsigned char *buf);
extern void RandrQueryOutputProperty(FD fd, const unsigned char *buf);
extern void RandrQueryOutputPropertyReply(FD fd, const unsigned char *buf);
extern void RandrConfigureOutputProperty(FD fd, const unsigned char *buf);
extern void RandrChangeOutputProperty(FD fd, const unsigned char *buf);
extern void RandrDeleteOutputProperty(FD fd, const unsigned char *buf);
extern void RandrGetOutputProperty(FD fd, const unsigned char *buf);
extern void RandrGetOutputPropertyReply(FD fd, const unsigned char *buf);
extern void RandrCreateMode(FD fd, const unsigned char *buf);
extern void RandrCreateModeReply(FD fd, const unsigned char *buf);
extern void RandrDestroyMode(FD fd, const unsigned char *buf);
extern void RandrAddOutputMode(FD fd, const unsigned char *buf);
extern void RandrDeleteOutputMode(FD fd, const unsigned char *buf);
extern void RandrGetCrtcInfo(FD fd, const unsigned char *buf);
extern void RandrGetCrtcInfoReply(FD fd, const unsigned char *buf);
extern void RandrSetCrtcConfig(FD fd, const unsigned char *buf);
extern void RandrSetCrtcConfigReply(FD fd, const unsigned char *buf);
extern void RandrGetCrtcGammaSize(FD fd, const unsigned char *buf);
extern void RandrGetCrtcGammaSizeReply(FD fd, const unsigned char *buf);
extern void RandrGetCrtcGamma(FD fd, const unsigned char *buf);
extern void RandrGetCrtcGammaReply(FD fd, const unsigned char *buf);
extern void RandrSetCrtcGamma(FD fd, const unsigned char *buf);

extern void RandrCrtcChangeNotifyEvent(const unsigned char *buf);
extern void RandrOutputChangeNotifyEvent(const unsigned char *buf);
extern void RandrOutputPropertyNotifyEvent(const unsigned char *buf);

extern void RandrOutputError(FD fd, const unsigned char *buf);
extern void RandrCrtcError(FD fd, const unsigned char *buf);
extern void RandrModeError(FD fd, const unsigned char *buf);

/* Randr 1.3 additions */
extern void RandrGetScreenResourcesCurrent(FD fd, const unsigned char *buf);
extern void RandrGetScreenResourcesCurrentReply(FD fd, const unsigned char *buf);
extern void RandrSetCrtcTransform(FD fd, const unsigned char *buf);
extern void RandrGetCrtcTransform(FD fd, const unsigned char *buf);
extern void RandrGetCrtcTransformReply(FD fd, const unsigned char *buf);
extern void RandrGetPanning(FD fd, const unsigned char *buf);
extern void RandrGetPanningReply(FD fd, const unsigned char *buf);
extern void RandrSetPanning(FD fd, const unsigned char *buf);
extern void RandrSetPanningReply(FD fd, const unsigned char *buf);
extern void RandrSetOutputPrimary(FD fd, const unsigned char *buf);
extern void RandrGetOutputPrimary(FD fd, const unsigned char *buf);
extern void RandrGetOutputPrimaryReply(FD fd, const unsigned char *buf);

/* Randr 1.4 additions */
extern void RandrGetProviders(FD fd, const unsigned char *buf);
extern void RandrGetProvidersReply(FD fd, const unsigned char *buf);
extern void RandrGetProviderInfo(FD fd, const unsigned char *buf);
extern void RandrGetProviderInfoReply(FD fd, const unsigned char *buf);
extern void RandrSetProviderOffloadSink(FD fd, const unsigned char *buf);
extern void RandrSetProviderOutputSource(FD fd, const unsigned char *buf);
extern void RandrListProviderProperties(FD fd, const unsigned char *buf);
extern void RandrListProviderPropertiesReply(FD fd, const unsigned char *buf);
extern void RandrQueryProviderProperty(FD fd, const unsigned char *buf);
extern void RandrQueryProviderPropertyReply(FD fd, const unsigned char *buf);
extern void RandrConfigureProviderProperty(FD fd, const unsigned char *buf);
extern void RandrChangeProviderProperty(FD fd, const unsigned char *buf);
extern void RandrDeleteProviderProperty(FD fd, const unsigned char *buf);
extern void RandrGetProviderProperty(FD fd, const unsigned char *buf);
extern void RandrGetProviderPropertyReply(FD fd, const unsigned char *buf);

extern void RandrProviderChangeNotifyEvent(const unsigned char *buf);
extern void RandrProviderPropertyNotifyEvent(const unsigned char *buf);
extern void RandrResourceChangeNotifyEvent(const unsigned char *buf);

extern void RandrProviderError(FD fd, const unsigned char *buf);

#endif
