#include "BADebugMenu.h"

#include "SGraphPanel.h"
#include "BlueprintAssist/BlueprintAssistUtils/BAMiscUtils.h"
#include "Components/VerticalBox.h"
#include "EdGraph/EdGraph.h"
#include "Framework/Docking/TabManager.h"
#include "MaterialGraph/MaterialGraphNode.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SEditableText.h"

void SBADebugMenuRow::Construct(const FArguments& InArgs)
{
	AddSlot().AutoWidth().HAlign(HAlign_Left).VAlign(VAlign_Center)
	[
		SNew(STextBlock).Text(InArgs._Label)
	];

	AddSlot().FillWidth(1.0f).HAlign(HAlign_Left).VAlign(VAlign_Center)
	[
		SNew(SEditableText).IsReadOnly(true).Text(InArgs._Value)
	];
}

void SBADebugMenu::Construct(const FArguments& InArgs)
{
	FocusedAssetEditor = FText::FromString("None");
	CurrentAsset = FText::FromString("None");
	GraphUnderCursor = FText::FromString("None");
	NodeUnderCursor = FText::FromString("None");
	NodeUnderCursorSize = FText::FromString("None");
	PinUnderCursor = FText::FromString("None");
	HoveredWidget = FText::FromString("None");
	FocusedWidget = FText::FromString("None");
	CurrentTab = FText::FromString("None");

	ChildSlot.VAlign(VAlign_Top).HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Asset Editor: "))
			.Value_Lambda([&] { return FocusedAssetEditor; })
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Asset: "))
			.Value_Lambda([&] { return CurrentAsset; })
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Graph: "))
			.Value_Lambda([&] { return GraphUnderCursor; })
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Node: "))
			.Value_Lambda([&] { return NodeUnderCursor; })
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Node Size: "))
			.Value_Lambda([&] { return NodeUnderCursorSize; })
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Pin: "))
			.Value_Lambda([&] { return PinUnderCursor; })
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Hovered Widget: "))
			.Value_Lambda([&] { return HoveredWidget; })
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Focused Widget: "))
			.Value_Lambda([&] { return FocusedWidget; })
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBADebugMenuRow)
			.Label(FText::FromString("Tab: "))
			.Value_Lambda([&] { return CurrentTab; })
		]
	];
}

void SBADebugMenu::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (IAssetEditorInstance* Editor = FBAUtils::GetEditorFromActiveTab())
	{
		FocusedAssetEditor = FText::FromString(Editor->GetEditorName().ToString());
	}

	if (UObject* Asset = FBAMiscUtils::GetAssetForActiveTab<UObject>())
	{
		CurrentAsset = FText::FromString(Asset->GetName());
	}

	if (auto Widget = FBAUtils::GetLastHoveredWidget())
	{
		auto Parent = Widget->GetParentWidget();
		FString ParentStr = Parent ? Parent->ToString() : "nullptr";
		HoveredWidget = FText::FromString(FString::Printf(TEXT("%s <%s>"), *Widget->ToString(), *ParentStr));
	}

	if (TSharedPtr<SWidget> Widget = FSlateApplication::Get().GetUserFocusedWidget(0))
	{
		FocusedWidget = FText::FromString(Widget->ToString());
	}

	if (auto Tab = FGlobalTabmanager::Get()->GetActiveTab())
	{
		CurrentTab = FText::FromString(Tab->GetTabLabel().ToString());
	}

	if (auto GraphPanel = FBAUtils::GetHoveredGraphPanel())
	{
		if (UEdGraph* EdGraph = GraphPanel->GetGraphObj())
		{
			GraphUnderCursor = FText::Format(INVTEXT("{0} ({1}) ({2})"), 
					FText::FromString(EdGraph->GetClass()->GetName()),
					FText::FromString(EdGraph->GraphGuid.ToString()),
					FText::FromString(FBAUtils::GetGraphGuid(EdGraph).ToString()));
		}

		if (auto GraphNode = FBAUtils::GetHoveredGraphNode(GraphPanel))
		{
			if (auto Node = GraphNode->GetNodeObj())
			{
				NodeUnderCursor = FText::Format(INVTEXT("{0} ({1}) ({2}) ({3}) ({4})"), 
					FText::FromString(Node->GetClass()->GetName()),
					FText::FromString(GetNameSafe(Node)),
					FText::FromString(FString::FromInt(Node->Pins.Num())),
					FText::FromString(Node->NodeGuid.ToString()),
					FText::FromString(FBAUtils::GetNodeGuid(Node).ToString()));

				NodeUnderCursorSize = FText::FromString(GraphNode->GetDesiredSize().ToString());
			}
		}

		if (TSharedPtr<SGraphPin> GraphPin = FBAUtils::GetHoveredGraphPin(GraphPanel))
		{
			if (UEdGraphPin* Pin = GraphPin->GetPinObj())
			{
				FText PinType = Pin->PinType.PinSubCategoryObject.IsValid()
					? FText::FromString(Pin->PinType.PinSubCategoryObject->GetName())
					: FText::FromString(Pin->PinType.PinCategory.ToString());

				PinUnderCursor = FText::Format(INVTEXT("{0} ({1})"),
					FText::FromString(FBAUtils::GetPinName(Pin)),
					PinType);
			}
		}
	}
}

void SBADebugMenu::RegisterNomadTab()
{
	const auto SpawnTab = FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
			.TabRole(ETabRole::NomadTab)
			[
				SNew(SBADebugMenu)
			];
	});

	FTabSpawnerEntry& Spawner = FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("BADebugMenu"), SpawnTab);
	Spawner.SetDisplayName(FText::FromString("Blueprint Assist Debug Menu"));
	Spawner.SetMenuType(ETabSpawnerMenuType::Hidden);
}
