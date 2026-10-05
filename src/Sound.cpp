// Vn.cpp


#include "stdafx.h"
#include "Sound.h"

CSound::CSound()
{
	InitVars();
}

CSound::CSound( LPCTSTR lpszFilename )
{
	InitVars();
	Load( lpszFilename );
}

CSound::CSound( LPCTSTR lpszResID, HINSTANCE hInstance )
{
	InitVars();
	Load( lpszResID, hInstance );
}

CSound::CSound( int nResID, HINSTANCE hInstance )
{
	InitVars();
	Load( nResID, hInstance );
}

void CSound::InitVars( void )
{
	m_bLoaded = FALSE;
	m_lpSoundData = NULL;
	m_hResHandle = NULL;
	m_nDevices = waveOutGetNumDevs();
}

CSound::~CSound()
{
	Close();
}

int CSound::DeviceCount( void )
{
	return( m_nDevices );
}

BOOL CSound::IsLoaded( void )
{
	return( m_bLoaded );
}

BOOL CSound::Load(LPCTSTR lpszFilename )
{
	Close();

	CFile File;
	if( !File.Open( lpszFilename, CFile::modeRead ) )
		return( FALSE );

	DWORD dwFileLength = (DWORD)File.GetLength();
	m_lpSoundData = new TCHAR [dwFileLength];
	if( m_lpSoundData == NULL )
		return( FALSE );

	if( File.Read( m_lpSoundData, dwFileLength )
		!= dwFileLength )
		return( FALSE );

	m_bLoaded = TRUE;

	return( TRUE );
}

BOOL CSound::Load( LPCTSTR lpszResID, HINSTANCE hInstance )
{
	Close();

	HANDLE hResInfo;
	hResInfo = FindResource ( hInstance, lpszResID, _T("WAVE") );
	if( hResInfo == NULL )
		return( FALSE );

	m_hResHandle = LoadResource( hInstance, (HRSRC) hResInfo );
	if( m_hResHandle == NULL )
		return( FALSE );

	m_lpSoundData = (LPTSTR ) LockResource( m_hResHandle );
	if( m_lpSoundData == NULL )
		return( FALSE );

	m_bLoaded = TRUE;

	return( TRUE );
}

BOOL CSound::Load( int nResID, HINSTANCE hInstance )
{
	Close();

	HANDLE hResInfo;
	hResInfo = FindResource ( hInstance,
		MAKEINTRESOURCE( nResID ), _T("WAVE") );
	if( hResInfo == NULL )
		return( FALSE );

	m_hResHandle = LoadResource( hInstance, (HRSRC) hResInfo );
	if( m_hResHandle == NULL )
		return( FALSE );

	m_lpSoundData = (LPTSTR ) LockResource( m_hResHandle );
	if( m_lpSoundData == NULL )
		return( FALSE );

	m_bLoaded = TRUE;

	return( TRUE );
}

BOOL CSound::Play( BOOL bLoop )
{
	if( !m_bLoaded )
		return( FALSE );

	Stop();
	
	DWORD dwFlags = SND_MEMORY | SND_ASYNC | SND_NODEFAULT;
	if( bLoop )
		dwFlags |= SND_LOOP;

	return( PlaySound( m_lpSoundData, NULL, dwFlags ) );
}

BOOL CSound::PlayFromDisk( LPCTSTR lpszFilename )
{
	Stop();
	
	return( PlaySound( lpszFilename, NULL,
		SND_FILENAME | SND_SYNC | SND_NODEFAULT ) );
}

BOOL CSound::PlayFromRes( LPCTSTR lpszResID,
						HINSTANCE hInstance )
{
	return( PlaySound( lpszResID, hInstance,
		SND_RESOURCE | SND_SYNC | SND_NODEFAULT ) );
}

BOOL CSound::PlayFromRes( int nResID, HINSTANCE hInstance )
{
	return( PlaySound( MAKEINTRESOURCE( nResID ),
		hInstance, SND_RESOURCE | SND_SYNC | SND_NODEFAULT ) );
}

BOOL CSound::Stop( void )
{
	return( PlaySound( NULL, NULL, NULL ) );
}

BOOL CSound::Close( void )
{
	Stop();

	if( m_hResHandle  != NULL ){
		UnlockResource( m_hResHandle );
		FreeResource( m_hResHandle );
		}
	else if( m_lpSoundData != NULL )
		delete [] m_lpSoundData;

	m_hResHandle = NULL;
	m_lpSoundData = NULL;

	return( TRUE );
}
