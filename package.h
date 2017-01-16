#ifndef PACKAGE_H
#define PACKAGE_H

const unsigned long long STR_SIZE = 512;

struct Package {
	bool IsMap;
	char Map[STR_SIZE];
	bool IsQuestion;
	char Cmd[STR_SIZE];
	char Message[STR_SIZE];
	bool IsAnswer;
	bool IsHit;
	bool Status;
	bool Ready;
	bool Exit;
	bool IsGameOver;
};

void SetEmptyPackage(Package *package);

#endif // PACKAGE_H
