//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
//
// GImageSink.h - MTK Image Sink Object Declaration
//

#ifndef __MTK_IMAGE_SINK_H__
#define __MTK_IAMGE_SINK_H__

#include <objbase.h>
#include <imaging.h>
#include "GDef.h"

#define PIXEL_FORMAT_MTK1   0X123456//PixelFormatMax + 100

///////////////////////////////////////////////////////////
//
// Image Sink
//
class MtkImageSink : public IImageSink, public IBitmapImage, public IImage, public IBasicBitmapOps
           
{
public:    
    // Creation
    //static HRESULT CreateInstance(IUnknown* pUnknownOuter,
                                  //CUnknown** ppNewComponent) ;

    // Declare the delegating IUnknown.
    //DECLARE_IUNKNOWN
    STDMETHOD (QueryInterface)(REFIID riid, void **ppv);
	ULONG AddRef();
	ULONG Release();
    // Nondelegating IUnknown
    //virtual HRESULT __stdcall 
        //NondelegatingQueryInterface( const IID& iid, void** ppv) ;
	
	MtkImageSink(
	UINT		   width,
	UINT		   height,
	PixelFormatID  pixelFormat,
	IImagingFactory *pImagFactory);
	
	MtkImageSink();
	~MtkImageSink();
/*-----------------------------IImageSink-------------------------*/
	STDMETHOD (BeginSink)(
			IN OUT ImageInfo* imageInfo,
            OUT OPTIONAL RECT* subarea
			);
	
	// End the sink process

	STDMETHOD(EndSink)(
		IN HRESULT statusCode
		);
	
	// Pass the color palette to the image sink

	STDMETHOD(SetPalette)(
		IN const ColorPalette* palette
		);
	
	// Ask the sink to allocate pixel data buffer

	STDMETHOD(GetPixelDataBuffer)(
		IN const RECT* rect,
		IN PixelFormatID pixelFormat,
		IN BOOL lastPass,
		OUT BitmapData* bitmapData
		);
	
	// Give the sink pixel data and release data buffer

	STDMETHOD(ReleasePixelDataBuffer)(
		IN const BitmapData* bitmapData
		);
	
	// Push pixel data

	STDMETHOD(PushPixelData)(
		IN const RECT* rect,
		IN const BitmapData* bitmapData,
		IN BOOL lastPass
		);
	
	// Push raw image data

	STDMETHOD(PushRawData)(
		IN const VOID* buffer,
		IN UINT bufsize
		);
	
	STDMETHOD(NeedTransform)(
		OUT UINT* rotation
		);
	
	STDMETHOD(NeedRawProperty)(
		);
		
	STDMETHOD(PushRawInfo)(
		IN OUT void* info
		);
	
	STDMETHOD(GetPropertyBuffer)(
		IN	   UINT 		   uiTotalBufferSize,
		IN OUT PropertyItem**  ppBuffer
		);
		
	STDMETHOD(PushPropertyItems)(
		IN UINT 			numOfItems,
		IN UINT 			uiTotalBufferSize,
		IN PropertyItem*	item
		);
/*-------------------------------------------------------------*/
/*------------------IBitmapImage-------------------------------*/
// Get bitmap dimensions in pixels

    STDMETHOD(GetSize)(
        OUT SIZE* size
        );

    // Get bitmap pixel format

    STDMETHOD(GetPixelFormatID)(
        OUT PixelFormatID* pixelFormat
        );

    // Access bitmap data in the specified pixel format
    //  must support at least PIXFMT_DONTCARE and
    //  the caninocal formats.

    STDMETHOD(LockBits)(
        IN const RECT* rect,
        IN UINT flags,
        IN PixelFormatID pixelFormat,
        IN OUT BitmapData* lockedBitmapData
        );

    STDMETHOD(UnlockBits)(
        IN const BitmapData* lockedBitmapData
        );

    // Set/get palette associated with the bitmap image

    STDMETHOD(GetPalette)(
        OUT ColorPalette** palette
        );
/*-------------------------------------------------------------*/
/*----------------IIMage---------------------------------------*/
// Get the device-independent physical dimension of the image
//	in unit of 0.01mm

STDMETHOD(GetPhysicalDimension)(
	OUT SIZE* size
	);

// Get basic image info

STDMETHOD(GetImageInfo)(
	OUT ImageInfo* imageInfo
	);

// Set image flags

STDMETHOD(SetImageFlags)(
	IN UINT flags
	);

// Display the image in a GDI device context

STDMETHOD(Draw)(
	IN HDC hdc,
	IN const RECT* dstRect,
	IN OPTIONAL const RECT* srcRect
	);

// Push image data into an IImageSink

STDMETHOD(PushIntoSink)(
	IN IImageSink* sink
	);

// Get a thumbnail representation for the image object

STDMETHOD(GetThumbnail)(
	IN OPTIONAL UINT thumbWidth,
	IN OPTIONAL UINT thumbHeight,
	OUT IImage** thumbImage
	);

/*-------------------------------------------------------------*/
/*----------------IBasicBitmapOps-------------------------------*/

HRESULT AdjustBrightness(
  FLOAT percent
);
HRESULT AdjustContrast(
  FLOAT shadow,
  FLOAT highlight
);
HRESULT AdjustGamma(
  FLOAT gamma
);
HRESULT Clone(
  OPTIONAL const RECT* rect,
  IBitmapImage**       outbmp,
  BOOL                 bNeedCloneProperty
);
HRESULT Flip(
  BOOL           flipX,
  BOOL           flipY,
  IBitmapImage** outbmp
);
HRESULT Resize(
  UINT              newWidth,
  UINT              newHeight,
  PixelFormatID     pixelFormat,
  InterpolationHint hints,
  IBitmapImage**    outbmp
);
HRESULT Rotate(
  FLOAT             angle,
  InterpolationHint hints,
  IBitmapImage**    outbmp
);


//private interface
HRESULT Hide();
HRESULT Show();

/*-------------------------------------------------------------*/

private:
	IImagingFactory *pImagingFactory;
	IBitmapImage *pDefaultBitmapImage;
	IImageSink *pImageSink;
	IImage *pImage;
    IBasicBitmapOps *m_pBmpOps;
	SIZE rImageSize;
	UINT32 m_u4Ref;
	GBOOL fgMTK1OverLay;
	GBOOL m_fgDrawed;
	GUINT8 *m_puPicData;
	GUINT8 *m_puRotateData;
	GBOOL fgRotated;
	GFLOAT m_fAngle;
	GUINT32 m_ImagePitch;
	GUINT32 m_OverlayPitch;
	RECT m_frontRect;

};

#endif

