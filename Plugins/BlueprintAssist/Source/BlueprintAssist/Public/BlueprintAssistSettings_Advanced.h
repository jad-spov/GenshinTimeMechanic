#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BlueprintAssistSettings_Advanced.generated.h"

UCLASS(config = EditorPerProjectUserSettings)
class BLUEPRINTASSIST_API UBASettings_Advanced final : public UObject
{
	GENERATED_BODY()

public:
	UBASettings_Advanced(const FObjectInitializer& ObjectInitializer);

	/* Fix for issue where copy-pasting material nodes will result in their material expressions having the same GUID */
	UPROPERTY(EditAnywhere, config, Category = Experimental)
	bool bGenerateUniqueGUIDForMaterialExpressions;

	UPROPERTY(EditAnywhere, config, Category = Experimental)
	bool bStoreCacheDataInPackageMetaData;

	UPROPERTY(EditAnywhere, config, Category = Default)
	TSet<FName> DisabledCommands;
};
