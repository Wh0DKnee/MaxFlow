#pragma once
#include "Visualizer.h"
class PushRelabelVis : public Visualizer
{
public:
	virtual void reset() override;

	virtual void step() override;

	virtual void runAlgorithm() override;

	static const float nodeRenderRadius;

private:
	bool isFirstStep = true;
};

