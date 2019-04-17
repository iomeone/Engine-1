//------------------------------------------------------------------------------------------------------------------------------
#include "Engine/Core/Clock.hpp"
// Engine Systems
#include "Engine/Commons/EngineCommon.hpp"
#include "Engine/Commons/ErrorWarningAssert.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Math/MathUtils.hpp"


STATIC Clock Clock::m_masterClock = Clock(nullptr);

//------------------------------------------------------------------------------------------------------------------------------
Clock::Clock()
{
	m_parent = &m_masterClock;
}

//------------------------------------------------------------------------------------------------------------------------------
Clock::Clock(Clock* parent)
{
	SetParent(parent);

	if(parent == nullptr)
	{
		m_totalTime = GetCurrentTimeSeconds();
	}
}

//------------------------------------------------------------------------------------------------------------------------------
Clock::~Clock()
{
	ASSERT_RECOVERABLE((m_children.size() == 0), "The clock being deleted had child Clock objects! Need to clean up before deleting this clock"); // other people should have cleaned up 
	SetParent(nullptr);
}

//------------------------------------------------------------------------------------------------------------------------------
void Clock::SetParent(Clock* parent)
{
	//Do you have a parent? If yes, remove me from it's child, then move me as child of my new parent
	if (m_parent != nullptr)
	{
		Clock* itrReference = nullptr;
		std::vector<Clock*>::iterator itr = m_parent->m_children.begin();
		while (itr != m_parent->m_children.end())
		{
			if (*itr == this)
			{
				itrReference = *itr;
			}
		}

		*itr = itrReference;
		m_parent->m_children.erase(itr);
		m_parent = nullptr;
	}

	m_parent = parent;

	if (m_parent != nullptr)
	{
		m_parent->m_children.push_back(this);
	}
}

//------------------------------------------------------------------------------------------------------------------------------
void Clock::SetFrameLimit(double frameLimit)
{
	m_frameLimit = frameLimit;
}

//------------------------------------------------------------------------------------------------------------------------------
void Clock::Step(float deltaSeconds)
{
	deltaSeconds *= m_dilation;

	//If paused simply set DeltaSeconds to 0
	if(IsPaused())
	{
		deltaSeconds = 0;
	}

	deltaSeconds = Clamp(deltaSeconds, 0.f, m_frameLimit);

	++m_framCount;
	m_totalTime += deltaSeconds;
	m_frameTime = deltaSeconds;

	//Call step on all the child clocks
	int numChildren = (int)m_children.size();
	for (int childIndex = 0; childIndex < numChildren; childIndex++)
	{
		m_children[childIndex]->Step(deltaSeconds);
	}
}
