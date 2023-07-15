#pragma once

#include "CoreMinimal.h"
#include "Layout/Margin.h"
#include "UObject/Object.h"
#include "BlueprintAssistSettings_EditorFeatures.generated.h"

UCLASS(Config = EditorPerProjectUserSettings)
class BLUEPRINTASSIST_API UBASettings_EditorFeatures final : public UObject
{
	GENERATED_BODY()

public:
	UBASettings_EditorFeatures(const FObjectInitializer& ObjectInitializer);

	////////////////////////////////////////////////////////////
	/// CustomEventReplication
	////////////////////////////////////////////////////////////

	/* Set the according replication flags after renaming a custom event by matching the prefixes below */
	UPROPERTY(EditAnywhere, Config, Category = CustomEventReplication)
	bool bSetReplicationFlagsAfterRenaming;

	/* When enabled, renaming a custom event with no matching prefix will apply "NotReplicated" */
	UPROPERTY(EditAnywhere, Config, Category = CustomEventReplication, meta=(EditCondition="bSetReplicationFlagsAfterRenaming"))
	bool bClearReplicationFlagsWhenRenamingWithNoPrefix;

	/* Add the according prefix to the title after changing replication flags */
	UPROPERTY(EditAnywhere, Config, Category = CustomEventReplication)
	bool bAddReplicationPrefixToCustomEventTitle;

	UPROPERTY(EditAnywhere, Config, Category = CustomEventReplication)
	FString MulticastPrefix;

	UPROPERTY(EditAnywhere, Config, Category = CustomEventReplication)
	FString ServerPrefix;

	UPROPERTY(EditAnywhere, Config, Category = CustomEventReplication)
	FString ClientPrefix;

	////////////////////////////////////////////////////////////
	/// Node group
	////////////////////////////////////////////////////////////

	/* Draw an outline to visualise each node group on the graph */
	UPROPERTY(EditAnywhere, Config, Category = NodeGroup)
	bool bDrawNodeGroupOutline;

	/* Only draw the group outline when selected */
	UPROPERTY(EditAnywhere, Config, Category = NodeGroup, meta=(EditCondition="bDrawNodeGroupOutline", EditConditionHides))
	bool bOnlyDrawGroupOutlineWhenSelected;

	/* Change the color of the border around the selected pin */
	UPROPERTY(EditAnywhere, Config, Category = NodeGroup, meta=(EditCondition="bDrawNodeGroupOutline", EditConditionHides))
	FLinearColor NodeGroupOutlineColor;

	/* Change the color of the border around the selected pin */
	UPROPERTY(EditAnywhere, Config, Category = NodeGroup, meta=(EditCondition="bDrawNodeGroupOutline", EditConditionHides))
	float NodeGroupOutlineWidth;

	/* Outline margin around each node */
	UPROPERTY(EditAnywhere, Config, Category = NodeGroup, meta=(EditCondition="bDrawNodeGroupOutline", EditConditionHides))
	FMargin NodeGroupOutlineMargin;

	/* Draw a fill to show the node groups for selected nodes */
	UPROPERTY(EditAnywhere, Category = NodeGroup)
	bool bDrawNodeGroupFill;

	/* Change the color of the border around the selected pin */
	UPROPERTY(EditAnywhere, Config, Category = NodeGroup, meta=(EditCondition="bDrawNodeGroupFill", EditConditionHides))
	FLinearColor NodeGroupFillColor;
};
