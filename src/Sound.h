// Sound.h

#ifndef __VN_H__
#define __VN_H__

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class CSound
{

public:
	CSound();
	CSound( LPCTSTR  );
	CSound( LPCTSTR , HINSTANCE );
	CSound( int, HINSTANCE );
	~CSound();

	int DeviceCount( void );

	BOOL Load( LPCTSTR  );
	BOOL Load( LPCTSTR , HINSTANCE );
	BOOL Load( int, HINSTANCE );

	BOOL Play( BOOL bLoop = FALSE );
	BOOL PlayFromDisk( LPCTSTR  );
	BOOL PlayFromRes( LPCTSTR , HINSTANCE );
	BOOL PlayFromRes( int, HINSTANCE );
	BOOL Stop( void );
	BOOL Close( void );

	BOOL IsLoaded( void );

protected:
	void InitVars( void );

	int m_nDevices;
	BOOL m_bLoaded;
	LPTSTR m_lpSoundData;
	HANDLE m_hResHandle;

};

#endif
