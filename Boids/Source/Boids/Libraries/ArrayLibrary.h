#pragma once

#include "CoreMinimal.h"

class ArrayLibrary {

public:
	template<typename T>
	static bool IsEmpty(const TArray<T>& Array) {
		return Array.Num() == 0;
	}
};
