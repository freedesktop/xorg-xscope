#include "x11.h"

/* common.c */
extern void enterprocedure (char *s);
extern void warn (char *s);
extern void panic (char *s);
extern void *Malloc (long n);
extern void Free (void *p);
extern void SetSignalHandling (void);
extern void SetUpConnectionSocket (int iport, void (*connectionFunc) (int));

/* decode11.c */
extern void InitReplyQ (void);
extern void FlushReplyQ (FD fd);
extern short CheckReplyTable (FD fd, short SequenceNumber, short *minorp);
extern void ReplyExpected (FD fd, short Request);
extern void ExtendedReplyExpected (FD fd, short Request, short RequestMinor);
extern void KeepLastReplyExpected (void);

extern void DecodeRequest	(FD fd, unsigned char *buf, long n);
extern void DecodeReply		(FD fd, unsigned char *buf, long n);
extern void DecodeError		(FD fd, unsigned char *buf, long n);
extern void DecodeEvent		(FD fd, unsigned char *buf, long n);

/* X11 Extension decoders in decode_*.c */
#define extension_decode_prototypes(ext) \
	extern void ext##_decode_req	(FD fd, unsigned char *buf);	\
	extern void ext##_decode_reply	(FD fd, unsigned char *buf,	\
					 short RequestMinor);		\
	extern void ext##_decode_error	(FD fd, unsigned char *buf);	\
	extern void ext##_decode_event	(FD fd, unsigned char *buf)

extension_decode_prototypes(bigreq);
extension_decode_prototypes(lbx);
extension_decode_prototypes(mitshm);
extension_decode_prototypes(randr);
extension_decode_prototypes(render);
extension_decode_prototypes(wcp);

extern void InitializeBIGREQ	(unsigned char *buf);
extern void InitializeLBX	(unsigned char *buf);
extern void InitializeMITSHM	(unsigned char *buf);
extern void InitializeRANDR	(unsigned char *buf);
extern void InitializeRENDER	(unsigned char *buf);
extern void InitializeWCP	(unsigned char *buf);

/* print11.c */
extern void PrintSetUpMessage (unsigned char *buf);
extern void PrintSetUpReply (unsigned char *buf);

extern void RequestError (unsigned char *buf);
extern void ValueError (unsigned char *buf);
extern void WindowError (unsigned char *buf);
extern void PixmapError (unsigned char *buf);
extern void AtomError (unsigned char *buf);
extern void CursorError (unsigned char *buf);
extern void FontError (unsigned char *buf);
extern void MatchError (unsigned char *buf);
extern void DrawableError (unsigned char *buf);
extern void AccessError (unsigned char *buf);
extern void AllocError (unsigned char *buf);
extern void ColormapError (unsigned char *buf);
extern void GContextError (unsigned char *buf);
extern void IDChoiceError (unsigned char *buf);
extern void NameError (unsigned char *buf);
extern void LengthError (unsigned char *buf);
extern void ImplementationError (unsigned char *buf);

extern void KeyPressEvent (unsigned char *buf);
extern void KeyReleaseEvent (unsigned char *buf);
extern void ButtonPressEvent (unsigned char *buf);
extern void ButtonReleaseEvent (unsigned char *buf);
extern void MotionNotifyEvent (unsigned char *buf);
extern void EnterNotifyEvent (unsigned char *buf);
extern void LeaveNotifyEvent (unsigned char *buf);
extern void FocusInEvent (unsigned char *buf);
extern void FocusOutEvent (unsigned char *buf);
extern void KeymapNotifyEvent (unsigned char *buf);
extern void ExposeEvent (unsigned char *buf);
extern void GraphicsExposureEvent (unsigned char *buf);
extern void NoExposureEvent (unsigned char *buf);
extern void VisibilityNotifyEvent (unsigned char *buf);
extern void CreateNotifyEvent (unsigned char *buf);
extern void DestroyNotifyEvent (unsigned char *buf);
extern void UnmapNotifyEvent (unsigned char *buf);
extern void MapNotifyEvent (unsigned char *buf);
extern void MapRequestEvent (unsigned char *buf);
extern void ReparentNotifyEvent (unsigned char *buf);
extern void ConfigureNotifyEvent (unsigned char *buf);
extern void ConfigureRequestEvent (unsigned char *buf);
extern void GravityNotifyEvent (unsigned char *buf);
extern void ResizeRequestEvent (unsigned char *buf);
extern void CirculateNotifyEvent (unsigned char *buf);
extern void CirculateRequestEvent (unsigned char *buf);
extern void PropertyNotifyEvent (unsigned char *buf);
extern void SelectionClearEvent (unsigned char *buf);
extern void SelectionRequestEvent (unsigned char *buf);
extern void SelectionNotifyEvent (unsigned char *buf);
extern void ColormapNotifyEvent (unsigned char *buf);
extern void ClientMessageEvent (unsigned char *buf);
extern void MappingNotifyEvent (unsigned char *buf);

extern void ExtendedRequest (int fd, unsigned char *buf);
extern void UnknownReply (unsigned char *buf);

extern void CreateWindow (unsigned char *buf);
extern void ChangeWindowAttributes (unsigned char *buf);
extern void GetWindowAttributes (unsigned char *buf);
extern void GetWindowAttributesReply (unsigned char *buf);
extern void DestroyWindow (unsigned char *buf);
extern void DestroySubwindows (unsigned char *buf);
extern void ChangeSaveSet (unsigned char *buf);
extern void ReparentWindow (unsigned char *buf);
extern void MapWindow (unsigned char *buf);
extern void MapSubwindows (unsigned char *buf);
extern void UnmapWindow (unsigned char *buf);
extern void UnmapSubwindows (unsigned char *buf);
extern void ConfigureWindow (unsigned char *buf);
extern void CirculateWindow (unsigned char *buf);
extern void GetGeometry (unsigned char *buf);
extern void GetGeometryReply (unsigned char *buf);
extern void QueryTree (unsigned char *buf);
extern void QueryTreeReply (unsigned char *buf);
extern void InternAtom (unsigned char *buf);
extern void InternAtomReply (unsigned char *buf);
extern void GetAtomName (unsigned char *buf);
extern void GetAtomNameReply (unsigned char *buf);
extern void ChangeProperty (unsigned char *buf);
extern void DeleteProperty (unsigned char *buf);
extern void GetProperty (unsigned char *buf);
extern void GetPropertyReply (unsigned char *buf);
extern void ListProperties (unsigned char *buf);
extern void ListPropertiesReply (unsigned char *buf);
extern void SetSelectionOwner (unsigned char *buf);
extern void GetSelectionOwner (unsigned char *buf);
extern void GetSelectionOwnerReply (unsigned char *buf);
extern void ConvertSelection (unsigned char *buf);
extern void SendEvent (unsigned char *buf);
extern void GrabPointer (unsigned char *buf);
extern void GrabPointerReply (unsigned char *buf);
extern void UngrabPointer (unsigned char *buf);
extern void GrabButton (unsigned char *buf);
extern void UngrabButton (unsigned char *buf);
extern void ChangeActivePointerGrab (unsigned char *buf);
extern void GrabKeyboard (unsigned char *buf);
extern void GrabKeyboardReply (unsigned char *buf);
extern void UngrabKeyboard (unsigned char *buf);
extern void GrabKey (unsigned char *buf);
extern void UngrabKey (unsigned char *buf);
extern void AllowEvents (unsigned char *buf);
extern void GrabServer (unsigned char *buf);
extern void UngrabServer (unsigned char *buf);
extern void QueryPointer (unsigned char *buf);
extern void QueryPointerReply (unsigned char *buf);
extern void GetMotionEvents (unsigned char *buf);
extern void GetMotionEventsReply (unsigned char *buf);
extern void TranslateCoordinates (unsigned char *buf);
extern void TranslateCoordinatesReply (unsigned char *buf);
extern void WarpPointer (unsigned char *buf);
extern void SetInputFocus (unsigned char *buf);
extern void GetInputFocus (unsigned char *buf);
extern void GetInputFocusReply (unsigned char *buf);
extern void QueryKeymap (unsigned char *buf);
extern void QueryKeymapReply (unsigned char *buf);
extern void OpenFont (unsigned char *buf);
extern void CloseFont (unsigned char *buf);
extern void QueryFont (unsigned char *buf);
extern void QueryFontReply (unsigned char *buf);
extern void QueryTextExtents (unsigned char *buf);
extern void QueryTextExtentsReply (unsigned char *buf);
extern void ListFonts (unsigned char *buf);
extern void ListFontsReply (unsigned char *buf);
extern void ListFontsWithInfo (unsigned char *buf);
extern void ListFontsWithInfoReply (unsigned char *buf);
extern void SetFontPath (unsigned char *buf);
extern void GetFontPath (unsigned char *buf);
extern void GetFontPathReply (unsigned char *buf);
extern void CreatePixmap (unsigned char *buf);
extern void FreePixmap (unsigned char *buf);
extern void CreateGC (unsigned char *buf);
extern void ChangeGC (unsigned char *buf);
extern void CopyGC (unsigned char *buf);
extern void SetDashes (unsigned char *buf);
extern void SetClipRectangles (unsigned char *buf);
extern void FreeGC (unsigned char *buf);
extern void ClearArea (unsigned char *buf);
extern void CopyArea (unsigned char *buf);
extern void CopyPlane (unsigned char *buf);
extern void PolyPoint (unsigned char *buf);
extern void PolyLine (unsigned char *buf);
extern void PolySegment (unsigned char *buf);
extern void PolyRectangle (unsigned char *buf);
extern void PolyArc (unsigned char *buf);
extern void FillPoly (unsigned char *buf);
extern void PolyFillRectangle (unsigned char *buf);
extern void PolyFillArc (unsigned char *buf);
extern void PutImage (unsigned char *buf);
extern void GetImage (unsigned char *buf);
extern void GetImageReply (unsigned char *buf);
extern void PolyText8 (unsigned char *buf);
extern void PolyText16 (unsigned char *buf);
extern void ImageText8 (unsigned char *buf);
extern void ImageText16 (unsigned char *buf);
extern void CreateColormap (unsigned char *buf);
extern void FreeColormap (unsigned char *buf);
extern void CopyColormapAndFree (unsigned char *buf);
extern void InstallColormap (unsigned char *buf);
extern void UninstallColormap (unsigned char *buf);
extern void ListInstalledColormaps (unsigned char *buf);
extern void ListInstalledColormapsReply (unsigned char *buf);
extern void AllocColor (unsigned char *buf);
extern void AllocColorReply (unsigned char *buf);
extern void AllocNamedColor (unsigned char *buf);
extern void AllocNamedColorReply (unsigned char *buf);
extern void AllocColorCells (unsigned char *buf);
extern void AllocColorCellsReply (unsigned char *buf);
extern void AllocColorPlanes (unsigned char *buf);
extern void AllocColorPlanesReply (unsigned char *buf);
extern void FreeColors (unsigned char *buf);
extern void StoreColors (unsigned char *buf);
extern void StoreNamedColor (unsigned char *buf);
extern void QueryColors (unsigned char *buf);
extern void QueryColorsReply (unsigned char *buf);
extern void LookupColor (unsigned char *buf);
extern void LookupColorReply (unsigned char *buf);
extern void CreateCursor (unsigned char *buf);
extern void CreateGlyphCursor (unsigned char *buf);
extern void FreeCursor (unsigned char *buf);
extern void RecolorCursor (unsigned char *buf);
extern void QueryBestSize (unsigned char *buf);
extern void QueryBestSizeReply (unsigned char *buf);
extern void QueryExtension (unsigned char *buf);
extern void QueryExtensionReply (unsigned char *buf);
extern void ListExtensions (unsigned char *buf);
extern void ListExtensionsReply (unsigned char *buf);
extern void ChangeKeyboardMapping (unsigned char *buf);
extern void GetKeyboardMapping (unsigned char *buf);
extern void GetKeyboardMappingReply (unsigned char *buf);
extern void ChangeKeyboardControl (unsigned char *buf);
extern void GetKeyboardControl (unsigned char *buf);
extern void GetKeyboardControlReply (unsigned char *buf);
extern void Bell (unsigned char *buf);
extern void ChangePointerControl (unsigned char *buf);
extern void GetPointerControl (unsigned char *buf);
extern void GetPointerControlReply (unsigned char *buf);
extern void SetScreenSaver (unsigned char *buf);
extern void GetScreenSaver (unsigned char *buf);
extern void GetScreenSaverReply (unsigned char *buf);
extern void ChangeHosts (unsigned char *buf);
extern void ListHosts (unsigned char *buf);
extern void ListHostsReply (unsigned char *buf);
extern void SetAccessControl (unsigned char *buf);
extern void SetCloseDownMode (unsigned char *buf);
extern void KillClient (unsigned char *buf);
extern void RotateProperties (unsigned char *buf);
extern void ForceScreenSaver (unsigned char *buf);
extern void SetPointerMapping (unsigned char *buf);
extern void SetPointerMappingReply (unsigned char *buf);
extern void GetPointerMapping (unsigned char *buf);
extern void GetPointerMappingReply (unsigned char *buf);
extern void SetModifierMapping (unsigned char *buf);
extern void SetModifierMappingReply (unsigned char *buf);
extern void GetModifierMapping (unsigned char *buf);
extern void GetModifierMappingReply (unsigned char *buf);
extern void NoOperation (unsigned char *buf);

/* prtype.c */
extern void SetIndentLevel (short which);
extern void DumpHexBuffer (unsigned char *buf, long n);
extern void DumpItem (char *name, FD fd, unsigned char *buf, long n);
extern void PrintField (unsigned char *buf, short start, short length,
			short FieldType, char *name);
extern int PrintBytes (unsigned char buf[], long number, char *name);
extern void PrintValues (unsigned char *control, int clength, int ctype,
			 unsigned char *values, char *name);
extern void PrintTextList8 (unsigned char *buf, int length, char *name);
extern void PrintTextList16 (unsigned char *buf, int length, char *name);

/* scope.c */
extern void TestBreakPoints (unsigned char *buf, long n);
extern void TimerExpired (void);
extern void SetUpPair (FD client, FD server);
extern FD FDPair (FD fd);
extern FD ClientHalf (FD fd);
extern FD ServerHalf (FD fd);
extern char *ClientName (FD fd);
extern int ClientNumber (FD fd);
extern void NewConnection (FD fd);
extern void FlushFD (FD fd);
extern short GetServerport (void);
extern void ReadCommands (void);

/* server.c */
extern FD ConnectToServer (Boolean report);
extern void ProcessBuffer (FD fd, unsigned char *buf, long n);
extern void ReportFromClient (FD fd, unsigned char *buf, long n);
extern void ReportFromServer (FD fd, unsigned char *buf, long n);
extern void PrintTime (void);
extern void SetBufLimit (FD fd);
extern void ClearBufLimit (FD fd);
extern void StartClientConnection (FD fd);
extern void StopClientConnection (FD fd);
extern void StartServerConnection (FD fd);
extern void StopServerConnection (FD fd);
extern long StartSetUpMessage (FD fd, unsigned char *buf, long n);
extern long StartSetUpReply (FD fd, unsigned char *buf, long n);

extern long GetXRequestFromName (char *name);

/* table11.c */
extern void InitializeX11 (void);
extern TYPE DefineType (short typeid, short class, char *name,
			int (*printproc)(unsigned char *));
extern void DefineEValue (TYPE type, long value, char *name);
extern void DefineValues (TYPE type, long value, short length,
			  short ctype, char *name);
extern long GetEValue (short typeid, char *name);
