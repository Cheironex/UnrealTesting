// Fill out your copyright notice in the Description page of Project Settings.


#include "TestWidget.h"

TSharedPtr<SWidget> UTestWidget::GetSlate()
{
    return this->GetCachedWidget();
}