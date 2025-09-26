// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Dom/JsonValue.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JsonUtilityManager.generated.h"

UCLASS()
class JSONUTILITYMANAGERPLUGIN_API AJsonUtilityManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJsonUtilityManager();

public:	
	UFUNCTION(BlueprintCallable, Category = "Json Utility Manager")
	static bool LoadJsonFileToString(const FString& FilePath, FString& OutJsonString);
	UFUNCTION(BlueprintCallable, Category = "Json Utility Manager")
	static FString GetJsonValueByKey(const FString& JsonString, const FString& Key);
	static FString GetJsonValueAsString(const TSharedPtr<FJsonValue>& JsonValuePtr);
};
