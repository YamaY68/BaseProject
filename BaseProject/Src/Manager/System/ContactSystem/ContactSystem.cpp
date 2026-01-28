#include "ContactSystem.h"

void ContactSystem::OnBeginContact(EntityID a, EntityID b, CollisionResult result)
{
	touching_[a].insert(b);
	touching_[b].insert(a);

	contactRules_.push_back(
		ContactRule{
			ContactEventInfo{
				ContactEventInfo::Type::BEGIN,
				a,
				b
			}
		});
}

void ContactSystem::OnEndContact(EntityID a, EntityID b, CollisionResult result)
{
	touching_[a].erase(b);
	touching_[b].erase(a);

	contactRules_.push_back(
		ContactRule{
			ContactEventInfo{
				ContactEventInfo::Type::END,
				a,
				b
			}
		});

}

