//��װ��BASS MIDI�е�����API������ͨ����̬���صķ�ʽ
#pragma once
typedef DWORD HSOUNDFONT;	// soundfont handle
struct BASS_MIDI_FONT 
{
	HSOUNDFONT font;	// soundfont
	int preset;			// preset number (-1=all)
	int bank;
};

class CBASSMidiLibrary
{
typedef HSOUNDFONT (WINAPI *_BASS_MIDI_FontInit)(const void* file, DWORD flags);
typedef BOOL (WINAPI *_BASS_MIDI_StreamSetFonts)(HSTREAM handle, void* fonts, DWORD count);
typedef BOOL (WINAPI *_BASS_MIDI_FontFree)(HSOUNDFONT handle);
public:
	CBASSMidiLibrary();
	~CBASSMidiLibrary();

	void Init(const wstring& dll_path);		//����DLL�ļ�����ȡ�������
	bool IsSuccessed();		//�ж�DLL�еĺ����Ƿ��ȡ�ɹ�

	_BASS_MIDI_FontInit BASS_MIDI_FontInit;
	_BASS_MIDI_StreamSetFonts BASS_MIDI_StreamSetFonts;
	_BASS_MIDI_FontFree BASS_MIDI_FontFree;
private:
	bool m_successed{ false };
};

