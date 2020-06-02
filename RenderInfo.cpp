#include "RenderInfo.h"
#include "Vertex.h"

void RenderInfo::highlight()
{
	isHighlighted = true;
	color = highlightColor;
}

void RenderInfo::resetHighlight()
{
	isHighlighted = false;
	color = regularColor;
}

void RenderInfo::setRegularColor(sf::Color c)
{
	regularColor = c;
	if (!isHighlighted)
	{
		color = c;
	}
}

void RenderInfo::setHighlightColor(sf::Color c)
{
	highlightColor = c;
	if (isHighlighted)
	{
		color = c;
	}
}

void VertexRenderInfo::reset()
{
	RenderInfo::resetHighlight();
	labelType = LabelType::NONE;
	shape = Shape::CIRCLE;
	radius = UIConfig::nodeRadius;
	squareLength = UIConfig::nodeSquareSize;
}

std::string VertexRenderInfo::getLabel(const Vertex& vert) const
{
	switch (labelType)
	{
	case VertexRenderInfo::NONE:
		return "";
		break;
	case VertexRenderInfo::HEIGHT:
		return std::to_string(vert.getHeight());
		break;
	default:
		return "";
		break;
	}
}
