#include <gtest/gtest.h>

#include <set>

#include "Common/Game/Utilities/IdGenerator.hpp"

TEST(IdGeneratorTest, GenerateFewIds)
{
    std::set<unsigned> ids;
    Common::Game::Utilities::IdGenerator idGenerator;

    for (int i = 0; i < 100; i++)
    {
        unsigned id = idGenerator.generate();
        auto ret = ids.insert(id);
        EXPECT_TRUE(id > 0);
        EXPECT_TRUE(ret.second);
    }
}

TEST(IdGeneratorTest, Reserve)
{
    std::set<unsigned> ids;
    Common::Game::Utilities::IdGenerator idGenerator;

    idGenerator.reserve(5);
    ids.insert(5);

    idGenerator.reserve(10);
    ids.insert(10);

    ASSERT_ANY_THROW(idGenerator.reserve(10));

    for (int i = 0; i < 100; i++)
    {
        unsigned id = idGenerator.generate();
        auto ret = ids.insert(id);
        EXPECT_TRUE(id > 0);
        EXPECT_TRUE(ret.second);
    }
}

