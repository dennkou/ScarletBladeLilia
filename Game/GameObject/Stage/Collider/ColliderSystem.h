#pragma once

class ColliderSystem
{
public:
	class InstanceCollection;
	class PlayerCollider;
	class PlayerAttackCollider;
	class EnemyCollider;
	class EnemyAttackCollider;
	class StageCollider;

	ColliderSystem() = delete;
	~ColliderSystem() = delete;

	static void Update();
private:
	static InstanceCollection m_instanceCollection;
};