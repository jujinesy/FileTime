#pragma once
#include <windows.h>
#include <stdio.h>
#include <winuser.h>
#include <time.h>
#include <Tchar.h>
#include <locale.h>
#include <stdlib.h>
#include <atlstr.h>

int main(int argc, char* argv[])
{
	if (argc == 1) {
		printf("C:\\>");
		exit(1);
	}

	HANDLE h_File = CreateFile(_T(argv[1]), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (h_File != INVALID_HANDLE_VALUE)
	{
		FILETIME create_time, write_time, access_time;
		SYSTEMTIME create_system_time, write_system_time, access_system_time;
		SYSTEMTIME create_local_time, write_local_time, access_local_time;

		GetFileTime(h_File, &create_time, &access_time, &write_time); 
		
		FileTimeToSystemTime(&create_time, &create_system_time);
		FileTimeToSystemTime(&write_time, &write_system_time);
		FileTimeToSystemTime(&access_time, &access_system_time);

		//systemtime�� �׳� ���� �ȵǰ� UTC(Universal Time Coordinated) ���� �̱� ������ �����ð����� ����
		//SystemTimeToTzSpecificLocalTime(NULL, &write_system_time, &write_local_time);

		create_local_time = create_system_time;
		write_local_time = write_system_time;
		access_local_time = access_system_time;

		FILE *fp;
		fopen_s(&fp, (CString)argv[1] + ".bak", "r");
		if (fp != NULL)
		{
			fscanf_s(fp, "%hu %hu %hu %hu %hu %hu %hu %hu",
				&create_local_time.wYear,
				&create_local_time.wMonth,
				&create_local_time.wDay,
				&create_local_time.wDayOfWeek,
				&create_local_time.wHour,
				&create_local_time.wMinute,
				&create_local_time.wSecond,
				&create_local_time.wMilliseconds);

			fscanf_s(fp, "%hu %hu %hu %hu %hu %hu %hu %hu",
				&write_local_time.wYear,
				&write_local_time.wMonth,
				&write_local_time.wDay,
				&write_local_time.wDayOfWeek,
				&write_local_time.wHour,
				&write_local_time.wMinute,
				&write_local_time.wSecond,
				&write_local_time.wMilliseconds);

			fscanf_s(fp, "%hu %hu %hu %hu %hu %hu %hu %hu",
				&access_local_time.wYear,
				&access_local_time.wMonth,
				&access_local_time.wDay,
				&access_local_time.wDayOfWeek,
				&access_local_time.wHour,
				&access_local_time.wMinute,
				&access_local_time.wSecond,
				&access_local_time.wMilliseconds);
		}
		else {
			fopen_s(&fp, (CString)argv[1] + ".bak", "w+");
			fprintf(fp, "%hu %hu %hu %hu %hu %hu %hu %hu\n",
				create_local_time.wYear,
				create_local_time.wMonth,
				create_local_time.wDay,
				create_local_time.wDayOfWeek,
				create_local_time.wHour,
				create_local_time.wMinute,
				create_local_time.wSecond,
				create_local_time.wMilliseconds);

			fprintf(fp, "%hu %hu %hu %hu %hu %hu %hu %hu\n",
				write_local_time.wYear,
				write_local_time.wMonth,
				write_local_time.wDay,
				write_local_time.wDayOfWeek,
				write_local_time.wHour,
				write_local_time.wMinute,
				write_local_time.wSecond,
				write_local_time.wMilliseconds);

			fprintf(fp, "%hu %hu %hu %hu %hu %hu %hu %hu",
				access_local_time.wYear,
				access_local_time.wMonth,
				access_local_time.wDay,
				access_local_time.wDayOfWeek,
				access_local_time.wHour,
				access_local_time.wMinute,
				access_local_time.wSecond,
				access_local_time.wMilliseconds);
		}
		fclose(fp);
		
		SystemTimeToFileTime(&create_local_time, &create_time);
		SystemTimeToFileTime(&write_local_time, &write_time);
		SystemTimeToFileTime(&access_local_time, &access_time);

		SetFileTime(h_File, &create_time, &access_time, &write_time);
		//���� �ð��� �� �غ���
		//CTime FileTime(write_local_time);  //-> System �� Ctime �������ڷ� �־��� �� �ִ�.
		//CTime tmCurrentTime = CTime::GetCurrentTime();
		//CTimeSpan tmSpan_interval = tmCurrentTime - FileTime;
		//// ���� �ð����� ���� �󸶳� ������ �˼� �մ�. CTimeSpan���� ������ ���ϴ� �� ������ �ְ���?
		CloseHandle(h_File);
	}
	printf("Success!");
	return 0;
}