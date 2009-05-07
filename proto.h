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

extern void DecodeRequest	(FD fd, const unsigned char *buf, long n);
extern void DecodeReply		(FD fd, const unsigned char *buf, long n);
extern void DecodeError		(FD fd, const unsigned char *buf, long n);
extern void DecodeEvent		(FD fd, const unsigned char *buf, long n);

/* X11 Extension decoders in decode_*.c */
#define extension_decode_prototypes(ext) \
	extern void ext##_decode_req	(FD fd, const unsigned char *buf); \
	extern void ext##_decode_reply	(FD fd, const unsigned char *buf,  \
					 short RequestMinor);		   \
	extern void ext##_decode_error	(FD fd, const unsigned char *buf); \
	extern void ext##_decode_event	(FD fd, const unsigned char *buf)

extension_decode_prototypes(bigreq);
extension_decode_prototypes(lbx);
extension_decode_prototypes(mitshm);
extension_decode_prototypes(randr);
extension_decode_prototypes(render);
extension_decode_prototypes(wcp);

extern void InitializeBIGREQ	(const unsigned char *buf);
extern void InitializeLBX	(const unsigned char *buf);
extern void InitializeMITSHM	(const unsigned char *buf);
extern void InitializeRANDR	(const unsigned char *buf);
extern void InitializeRENDER	(const unsigned char *buf);
extern void InitializeWCP	(const unsigned char *buf);

/* print11.c */
extern void PrintSetUpMessage (const unsigned char *buf);
extern void PrintSetUpReply (const unsigned char *buf);

extern void RequestError (const unsigned char *buf);
extern void ValueError (const unsigned char *buf);
extern void WindowError (const unsigned char *buf);
extern void PixmapError (const unsigned char *buf);
extern void AtomError (const unsigned char *buf);
extern void CursorError (const unsigned char *buf);
extern void FontError (const unsigned char *buf);
extern void MatchError (const unsigned char *buf);
extern void DrawableError (const unsigned char *buf);
extern void AccessError (const unsigned char *buf);
extern void AllocError (const unsigned char *buf);
extern void ColormapError (const unsigned char *buf);
extern void GContextError (const unsigned char *buf);
extern void IDChoiceError (const unsigned char *buf);
extern void NameError (const unsigned char *buf);
extern void LengthError (const unsigned char *buf);
extern void ImplementationError (const unsigned char *buf);

extern void KeyPressEvent (const unsigned char *buf);
extern void KeyReleaseEvent (const unsigned char *buf);
extern void ButtonPressEvent (const unsigned char *buf);
extern void ButtonReleaseEvent (const unsigned char *buf);
extern void MotionNotifyEvent (const unsigned char *buf);
extern void EnterNotifyEvent (const unsigned char *buf);
extern void LeaveNotifyEvent (const unsigned char *buf);
extern void FocusInEvent (const unsigned char *buf);
extern void FocusOutEvent (const unsigned char *buf);
extern void KeymapNotifyEvent (const unsigned char *buf);
extern void ExposeEvent (const unsigned char *buf);
extern void GraphicsExposureEvent (const unsigned char *buf);
extern void NoExposureEvent (const unsigned char *buf);
extern void VisibilityNotifyEvent (const unsigned char *buf);
extern void CreateNotifyEvent (const unsigned char *buf);
extern void DestroyNotifyEvent (const unsigned char *buf);
extern void UnmapNotifyEvent (const unsigned char *buf);
extern void MapNotifyEvent (const unsigned char *buf);
extern void MapRequestEvent (const unsigned char *buf);
extern void ReparentNotifyEvent (const unsigned char *buf);
extern void ConfigureNotifyEvent (const unsigned char *buf);
extern void ConfigureRequestEvent (const unsigned char *buf);
extern void GravityNotifyEvent (const unsigned char *buf);
extern void ResizeRequestEvent (const unsigned char *buf);
extern void CirculateNotifyEvent (const unsigned char *buf);
extern void CirculateRequestEvent (const unsigned char *buf);
extern void PropertyNotifyEvent (const unsigned char *buf);
extern void SelectionClearEvent (const unsigned char *buf);
extern void SelectionRequestEvent (const unsigned char *buf);
extern void SelectionNotifyEvent (const unsigned char *buf);
extern void ColormapNotifyEvent (const unsigned char *buf);
extern void ClientMessageEvent (const unsigned char *buf);
extern void MappingNotifyEvent (const unsigned char *buf);

extern void ExtendedRequest (int fd, const unsigned char *buf);
extern void UnknownReply (const unsigned char *buf);

extern void CreateWindow (const unsigned char *buf);
extern void ChangeWindowAttributes (const unsigned char *buf);
extern void GetWindowAttributes (const unsigned char *buf);
extern void GetWindowAttributesReply (const unsigned char *buf);
extern void DestroyWindow (const unsigned char *buf);
extern void DestroySubwindows (const unsigned char *buf);
extern void ChangeSaveSet (const unsigned char *buf);
extern void ReparentWindow (const unsigned char *buf);
extern void MapWindow (const unsigned char *buf);
extern void MapSubwindows (const unsigned char *buf);
extern void UnmapWindow (const unsigned char *buf);
extern void UnmapSubwindows (const unsigned char *buf);
extern void ConfigureWindow (const unsigned char *buf);
extern void CirculateWindow (const unsigned char *buf);
extern void GetGeometry (const unsigned char *buf);
extern void GetGeometryReply (const unsigned char *buf);
extern void QueryTree (const unsigned char *buf);
extern void QueryTreeReply (const unsigned char *buf);
extern void InternAtom (const unsigned char *buf);
extern void InternAtomReply (const unsigned char *buf);
extern void GetAtomName (const unsigned char *buf);
extern void GetAtomNameReply (const unsigned char *buf);
extern void ChangeProperty (const unsigned char *buf);
extern void DeleteProperty (const unsigned char *buf);
extern void GetProperty (const unsigned char *buf);
extern void GetPropertyReply (const unsigned char *buf);
extern void ListProperties (const unsigned char *buf);
extern void ListPropertiesReply (const unsigned char *buf);
extern void SetSelectionOwner (const unsigned char *buf);
extern void GetSelectionOwner (const unsigned char *buf);
extern void GetSelectionOwnerReply (const unsigned char *buf);
extern void ConvertSelection (const unsigned char *buf);
extern void SendEvent (const unsigned char *buf);
extern void GrabPointer (const unsigned char *buf);
extern void GrabPointerReply (const unsigned char *buf);
extern void UngrabPointer (const unsigned char *buf);
extern void GrabButton (const unsigned char *buf);
extern void UngrabButton (const unsigned char *buf);
extern void ChangeActivePointerGrab (const unsigned char *buf);
extern void GrabKeyboard (const unsigned char *buf);
extern void GrabKeyboardReply (const unsigned char *buf);
extern void UngrabKeyboard (const unsigned char *buf);
extern void GrabKey (const unsigned char *buf);
extern void UngrabKey (const unsigned char *buf);
extern void AllowEvents (const unsigned char *buf);
extern void GrabServer (const unsigned char *buf);
extern void UngrabServer (const unsigned char *buf);
extern void QueryPointer (const unsigned char *buf);
extern void QueryPointerReply (const unsigned char *buf);
extern void GetMotionEvents (const unsigned char *buf);
extern void GetMotionEventsReply (const unsigned char *buf);
extern void TranslateCoordinates (const unsigned char *buf);
extern void TranslateCoordinatesReply (const unsigned char *buf);
extern void WarpPointer (const unsigned char *buf);
extern void SetInputFocus (const unsigned char *buf);
extern void GetInputFocus (const unsigned char *buf);
extern void GetInputFocusReply (const unsigned char *buf);
extern void QueryKeymap (const unsigned char *buf);
extern void QueryKeymapReply (const unsigned char *buf);
extern void OpenFont (const unsigned char *buf);
extern void CloseFont (const unsigned char *buf);
extern void QueryFont (const unsigned char *buf);
extern void QueryFontReply (const unsigned char *buf);
extern void QueryTextExtents (const unsigned char *buf);
extern void QueryTextExtentsReply (const unsigned char *buf);
extern void ListFonts (const unsigned char *buf);
extern void ListFontsReply (const unsigned char *buf);
extern void ListFontsWithInfo (const unsigned char *buf);
extern void ListFontsWithInfoReply (const unsigned char *buf);
extern void SetFontPath (const unsigned char *buf);
extern void GetFontPath (const unsigned char *buf);
extern void GetFontPathReply (const unsigned char *buf);
extern void CreatePixmap (const unsigned char *buf);
extern void FreePixmap (const unsigned char *buf);
extern void CreateGC (const unsigned char *buf);
extern void ChangeGC (const unsigned char *buf);
extern void CopyGC (const unsigned char *buf);
extern void SetDashes (const unsigned char *buf);
extern void SetClipRectangles (const unsigned char *buf);
extern void FreeGC (const unsigned char *buf);
extern void ClearArea (const unsigned char *buf);
extern void CopyArea (const unsigned char *buf);
extern void CopyPlane (const unsigned char *buf);
extern void PolyPoint (const unsigned char *buf);
extern void PolyLine (const unsigned char *buf);
extern void PolySegment (const unsigned char *buf);
extern void PolyRectangle (const unsigned char *buf);
extern void PolyArc (const unsigned char *buf);
extern void FillPoly (const unsigned char *buf);
extern void PolyFillRectangle (const unsigned char *buf);
extern void PolyFillArc (const unsigned char *buf);
extern void PutImage (const unsigned char *buf);
extern void GetImage (const unsigned char *buf);
extern void GetImageReply (const unsigned char *buf);
extern void PolyText8 (const unsigned char *buf);
extern void PolyText16 (const unsigned char *buf);
extern void ImageText8 (const unsigned char *buf);
extern void ImageText16 (const unsigned char *buf);
extern void CreateColormap (const unsigned char *buf);
extern void FreeColormap (const unsigned char *buf);
extern void CopyColormapAndFree (const unsigned char *buf);
extern void InstallColormap (const unsigned char *buf);
extern void UninstallColormap (const unsigned char *buf);
extern void ListInstalledColormaps (const unsigned char *buf);
extern void ListInstalledColormapsReply (const unsigned char *buf);
extern void AllocColor (const unsigned char *buf);
extern void AllocColorReply (const unsigned char *buf);
extern void AllocNamedColor (const unsigned char *buf);
extern void AllocNamedColorReply (const unsigned char *buf);
extern void AllocColorCells (const unsigned char *buf);
extern void AllocColorCellsReply (const unsigned char *buf);
extern void AllocColorPlanes (const unsigned char *buf);
extern void AllocColorPlanesReply (const unsigned char *buf);
extern void FreeColors (const unsigned char *buf);
extern void StoreColors (const unsigned char *buf);
extern void StoreNamedColor (const unsigned char *buf);
extern void QueryColors (const unsigned char *buf);
extern void QueryColorsReply (const unsigned char *buf);
extern void LookupColor (const unsigned char *buf);
extern void LookupColorReply (const unsigned char *buf);
extern void CreateCursor (const unsigned char *buf);
extern void CreateGlyphCursor (const unsigned char *buf);
extern void FreeCursor (const unsigned char *buf);
extern void RecolorCursor (const unsigned char *buf);
extern void QueryBestSize (const unsigned char *buf);
extern void QueryBestSizeReply (const unsigned char *buf);
extern void QueryExtension (const unsigned char *buf);
extern void QueryExtensionReply (const unsigned char *buf);
extern void ListExtensions (const unsigned char *buf);
extern void ListExtensionsReply (const unsigned char *buf);
extern void ChangeKeyboardMapping (const unsigned char *buf);
extern void GetKeyboardMapping (const unsigned char *buf);
extern void GetKeyboardMappingReply (const unsigned char *buf);
extern void ChangeKeyboardControl (const unsigned char *buf);
extern void GetKeyboardControl (const unsigned char *buf);
extern void GetKeyboardControlReply (const unsigned char *buf);
extern void Bell (const unsigned char *buf);
extern void ChangePointerControl (const unsigned char *buf);
extern void GetPointerControl (const unsigned char *buf);
extern void GetPointerControlReply (const unsigned char *buf);
extern void SetScreenSaver (const unsigned char *buf);
extern void GetScreenSaver (const unsigned char *buf);
extern void GetScreenSaverReply (const unsigned char *buf);
extern void ChangeHosts (const unsigned char *buf);
extern void ListHosts (const unsigned char *buf);
extern void ListHostsReply (const unsigned char *buf);
extern void SetAccessControl (const unsigned char *buf);
extern void SetCloseDownMode (const unsigned char *buf);
extern void KillClient (const unsigned char *buf);
extern void RotateProperties (const unsigned char *buf);
extern void ForceScreenSaver (const unsigned char *buf);
extern void SetPointerMapping (const unsigned char *buf);
extern void SetPointerMappingReply (const unsigned char *buf);
extern void GetPointerMapping (const unsigned char *buf);
extern void GetPointerMappingReply (const unsigned char *buf);
extern void SetModifierMapping (const unsigned char *buf);
extern void SetModifierMappingReply (const unsigned char *buf);
extern void GetModifierMapping (const unsigned char *buf);
extern void GetModifierMappingReply (const unsigned char *buf);
extern void NoOperation (const unsigned char *buf);

/* prtype.c */
extern void SetIndentLevel (short which);
extern void DumpHexBuffer (const unsigned char *buf, long n);
extern void DumpItem (const char *name, FD fd,
		      const unsigned char *buf, long n);
extern void PrintField (const unsigned char *buf, short start, short length,
			short FieldType, const char *name);
extern int PrintBytes (const unsigned char *buf, long number,
		       const char *name);
extern void PrintValues (const unsigned char *control, int clength, int ctype,
			 const unsigned char *values, const char *name);
extern void PrintTextList8 (const unsigned char *buf, int length,
			    const char *name);
extern void PrintTextList16 (const unsigned char *buf, int length,
			     const char *name);

/* scope.c */
extern void TestBreakPoints (const unsigned char *buf, long n);
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
extern void ProcessBuffer (FD fd, const unsigned char *buf, long n);
extern void ReportFromClient (FD fd, const unsigned char *buf, long n);
extern void ReportFromServer (FD fd, const unsigned char *buf, long n);
extern void PrintTime (void);
extern void SetBufLimit (FD fd);
extern void ClearBufLimit (FD fd);
extern void StartClientConnection (FD fd);
extern void StopClientConnection (FD fd);
extern void StartServerConnection (FD fd);
extern void StopServerConnection (FD fd);
extern long StartSetUpMessage (FD fd, const unsigned char *buf, long n);
extern long StartSetUpReply (FD fd, const unsigned char *buf, long n);

extern long GetXRequestFromName (const char *name);

/* table11.c */
extern void InitializeX11 (void);
extern TYPE DefineType (short typeid, short class, const char *name,
			int (*printproc)(const unsigned char *));
extern void DefineEValue (TYPE type, long value, const char *name);
extern void DefineValues (TYPE type, long value, short length,
			  short ctype, const char *name);
extern long GetEValue (short typeid, const char *name);
