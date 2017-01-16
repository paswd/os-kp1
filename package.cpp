 #include "package.h"

 void SetEmptyPackage(Package *package) {
	package->IsMap = false;
	package->Map[0] = '\0';
	package->IsQuestion = false;
	package->IsAnswer = false;
	package->IsHit = false;
	package->Status = false;
	package->Ready = false;
	package->Exit = false;
	package->IsGameOver = false;
}