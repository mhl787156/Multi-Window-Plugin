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


#pragma once



class SMultiWindow final : public SWindow
{
	SLATE_BEGIN_ARGS(SMultiWindow) {}
	SLATE_ARGUMENT(FName, WindowTitle);
	SLATE_ARGUMENT(FVector2D, WindowPosition);
	SLATE_ARGUMENT(FVector2D, WindowSize);
	SLATE_ARGUMENT(bool, SupportsMaximize);
	SLATE_ARGUMENT(bool, SupportsMinimize);
	SLATE_ARGUMENT(ESizingRule, SizingRool);
	SLATE_ARGUMENT(bool, SaneWindowPlacement);
	SLATE_ARGUMENT(bool, CreateTitleBar);
	SLATE_ARGUMENT(bool, FocusWhenFirstShown);
	SLATE_ARGUMENT(EAutoCenter, AutoCenter);
	SLATE_ARGUMENT(bool, UseOSWindowBorder);
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);


private:

};