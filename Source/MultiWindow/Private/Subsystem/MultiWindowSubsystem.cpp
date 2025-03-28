/*
MIT License

Copyright (c) DrElliot

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Subsystem/MultiWindowSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Windows/MW_Window.h"

DEFINE_LOG_CATEGORY(LogMultiWindow);

void UMultiWindowSubsystem::Deinitialize()
{
	for (auto& KVP : ActiveWindows)
	{
		KVP.Value->Shutdown();
	}

	Super::Deinitialize();
}

UMW_Window* UMultiWindowSubsystem::CreateMultiWindow(UObject* WorldContextObject,
													TSoftClassPtr<UUserWidget> WidgetClass,
													FName InTitle,
													EMultiWidgetDependencyType DependencyType,
													FVector2D WindowPosition,
													FVector2D WindowSize,
													UObject* DependencyObject,
													EBPSizingRule SizingRule,
													bool bSupportsMaximize,
													bool bSupportsMinimize,
													bool bSaneWindowPlacement,
													bool bCreateTitleBar,
													bool bFocusWhenFirstShown,
													bool bBorderlessWindow,
													uint8 display_id,
													bool bNativeAutoFullScreen,
													bool bWindowPositionRelativeToDisplay)
{
	check(!WidgetClass.IsNull());
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);


	// Get Monitor Information
	FDisplayMetrics DisplayMetrics;
	FSlateApplication::Get().GetDisplayMetrics(DisplayMetrics);

	int32 NumMonitors = DisplayMetrics.MonitorInfo.Num();
	if (NumMonitors < display_id)
	{
		UE_LOG(LogTemp, Warning, TEXT("Only %d monitors detected! Cannot position window %d"), NumMonitors, display_id);
		return nullptr;
	}
	// Get the position of the second monitor
	FMonitorInfo MonitorInfo= DisplayMetrics.MonitorInfo[display_id];  // Index 1 = Second Monitor

	if(bNativeAutoFullScreen)
	{
		// Set Window Size to the size of the monitor
		WindowSize = FVector2D(MonitorInfo.NativeWidth, MonitorInfo.NativeHeight);
	}

	if(bWindowPositionRelativeToDisplay)
	{
		// Set Window Position to be relative to the window work area
		WindowPosition = FVector2D(MonitorInfo.WorkArea.Left, MonitorInfo.WorkArea.Top) + WindowPosition;
	}

	UMW_Window* NewWindow = NewObject<UMW_Window>(&Get(), UMW_Window::StaticClass());
	NewWindow->WindowTitle = InTitle;
	NewWindow->WindowPosition = WindowPosition;
	NewWindow->WindowSize = WindowSize;
	NewWindow->SizingRule = SizingRule;
	NewWindow->bSupportsMaximize = bSupportsMaximize;
	NewWindow->bSupportsMinimize = bSupportsMinimize;
	NewWindow->DependencyType = DependencyType;
	NewWindow->DependencyObject = DependencyObject;
	NewWindow->bSaneWindowPlacement = bSaneWindowPlacement;
	NewWindow->bCreateTitleBar = bCreateTitleBar;
	NewWindow->bFocusWhenFirstShown = bFocusWhenFirstShown;
	// NewWindow->AutoCenter = AutoCenter;
	NewWindow->bBorderlessWindow = bBorderlessWindow;
	NewWindow->Init();

	Get().ActiveWindows.Add(InTitle, NewWindow);

	APlayerController* PC = World->GetFirstPlayerController();
	if(UUserWidget* UserWidget = CreateWidget(PC, WidgetClass.LoadSynchronous()))
	{
		AddWidgetToWindow(NewWindow, UserWidget);
	}

	return NewWindow;
}

bool UMultiWindowSubsystem::ShutdownWindowByName(FName WindowTitle)
{
	if(UMW_Window* Window = Get().ActiveWindows.FindRef(WindowTitle))
	{
		Window->Shutdown();
		Get().ActiveWindows.Remove(WindowTitle);
		return true;
	}
	return false;
}

bool UMultiWindowSubsystem::ShutdownWindowByObjectReference(UMW_Window* Window)
{
	Window->Shutdown();
	Get().ActiveWindows.Remove(Window->WindowTitle);

	return true;
}

UMW_Window* UMultiWindowSubsystem::AddWidgetToWindow(UMW_Window* InWindow, UUserWidget* InWidget)
{
	InWindow->AddWidgetToWindow(InWidget);
	return InWindow;
}

bool UMultiWindowSubsystem::IsWindowActive(FName Name)
{
	return Get().ActiveWindows.Contains(Name);
}

TArray<UMW_Window*> UMultiWindowSubsystem::GetActiveWindows()
{
	TArray<TObjectPtr<UMW_Window>> Windows;
	Get().ActiveWindows.GenerateValueArray(Windows);

	return Windows;
}

void UMultiWindowSubsystem::NotifyWindowClosedExternally_Internal(const UMW_Window* Window, bool bForced)
{
	ActiveWindows.Remove(Window->WindowTitle);

	UE_LOG(LogMultiWindow, Verbose, TEXT("Window: [%s] closing from external sources..."), *Window->WindowTitle.ToString());
}
