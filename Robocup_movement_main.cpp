/**
 * @file Lab34PlayingMidfieldCard.cpp
 *
 * This file implements the forward player for lab3,4
 */

#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Communication/RobotInfo.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"
#include "Tools/Math/BHMath.h"

CARD(EE308Lab34PlayingMidfieldCard,
{,
  CALLS(Activity),
  CALLS(LookActive),
  CALLS(Stand),
  CALLS(WalkToPoint),
  CALLS(GoToBallAndKick),

  REQUIRES(FieldBall),
  REQUIRES(FieldDimensions),
  REQUIRES(RobotPose),
  REQUIRES(RobotInfo),
  
  DEFINES_PARAMETERS(
  {,
    (int)(500) initialWaitTime, // ms
    (Vector2f)(Vector2f(0.0f, 0.0f)) passToForwardTargetOnField, // TODO adjust values
    (Pose2f)(Pose2f(0_deg, 0.0f, 0.0f)) passBackPoseOnField, // TODO adjust values
    // TODO add other parameters here if needed
  }),
});

class EE308Lab34PlayingMidfieldCard : public EE308Lab34PlayingMidfieldCardBase
{
  Vector2f kickTarget;

  bool preconditions() const override
  {
    // This means that this card is only runnable for robot4
    return (theRobotInfo.number == 4);
  }

  bool postconditions() const override
  {
    return true;
  }

  option
  {
    theActivitySkill(BehaviorStatus::ee308Playing);

    initial_state(start)
    {
      transition
      {
        if(state_time > initialWaitTime)
          goto done;
      }

      action
      {
        theLookActiveSkill(false, /* ignoreBall: */ true);
        theStandSkill();
      }
    }

    state(done)
    {
      action
      {
        theLookActiveSkill();
        theStandSkill();
      }
    }

  } // end option
};

MAKE_CARD(EE308Lab34PlayingMidfieldCard);

