#ifndef TEST_OBJECT_HPP
#define TEST_OBJECT_HPP

#include "game_object.hpp"


class TestObject : public virtual GameObject {
    public:
        int id;
        Vector2 position;

        TestObject(Vector2 position) {
            this->position = position;
        };

        void Update(float deltaTime) override;
        void Draw() override;
        bool CheckInWall();

};

#endif