// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonUtilityManager.h"
#include "Misc/FileHelper.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

// Sets default values
AJsonUtilityManager::AJsonUtilityManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AJsonUtilityManager::LoadJsonFileToString(const FString& FilePath, FString& OutJsonString)
{
	return FFileHelper::LoadFileToString(OutJsonString, *FilePath);
}

FString AJsonUtilityManager::GetJsonValueByKey(const FString& JsonString, const FString& Key)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// "/"로 구분된 Key 경로 처리
		TArray<FString> Keys;
		Key.ParseIntoArray(Keys, TEXT("/"), true);

		TSharedPtr<FJsonValue> JsonValuePtr;
		TSharedPtr<FJsonObject> CurrentObject = JsonObject;

		for (int32 i = 0; i < Keys.Num(); i++)
		{
			JsonValuePtr = CurrentObject->TryGetField(Keys[i]);
			if (!JsonValuePtr.IsValid())
			{
				return FString();
			}

			if (i < Keys.Num() - 1)
			{
				if (JsonValuePtr->Type == EJson::Object)
				{
					CurrentObject = JsonValuePtr->AsObject();
				}
				else
				{
					return FString();
				}
			}
		}

		// 최종 JsonValuePtr에 대한 처리
		if (JsonValuePtr.IsValid())
		{
			switch (JsonValuePtr->Type)
			{
			case EJson::String:
				return JsonValuePtr->AsString();
			case EJson::Number:
				return FString::Printf(TEXT("%f"), JsonValuePtr->AsNumber());
			case EJson::Boolean:
				return JsonValuePtr->AsBool() ? TEXT("true") : TEXT("false");
			case EJson::Array:
				{
					TArray<TSharedPtr<FJsonValue>> Array = JsonValuePtr->AsArray();
					FString Result = TEXT("[");
					for (auto& Elem : Array)
					{
						Result += GetJsonValueAsString(Elem) + TEXT(", ");
					}
					Result.RemoveFromEnd(TEXT(", "));
					Result += TEXT("]");
					return Result;
				}
			case EJson::Object:
				return TEXT("Object data type not directly supported.");
			default:
				return TEXT("Unknown data type.");
			}
		}
	}

	return FString();
}

FString AJsonUtilityManager::GetJsonValueAsString(const TSharedPtr<FJsonValue>& JsonValuePtr)
{
	switch (JsonValuePtr->Type)
	{
	case EJson::String:
		return JsonValuePtr->AsString();
	case EJson::Number:
		return FString::Printf(TEXT("%f"), JsonValuePtr->AsNumber());
	case EJson::Boolean:
		return JsonValuePtr->AsBool() ? TEXT("true") : TEXT("false");
	case EJson::Array:
		{
			TArray<TSharedPtr<FJsonValue>> Array = JsonValuePtr->AsArray();
			FString Result = TEXT("[");
			for (auto& Elem : Array)
			{
				Result += GetJsonValueAsString(Elem) + TEXT(", ");
			}
			Result.RemoveFromEnd(TEXT(", "));
			Result += TEXT("]");
			return Result;
		}
	case EJson::Object:
		return TEXT("{Object data}");
	default:
		return TEXT("Unknown data type.");
	}
}

