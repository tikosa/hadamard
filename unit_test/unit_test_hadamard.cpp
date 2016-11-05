#include "hadamard.h"
#include <vector>

#include <gtest/gtest.h>

namespace {

TEST(Hadamard, hadamard_matrix) {
    std::unique_ptr<std::vector<int> > h_0 = hadamard<std::vector<int> >(0);
    std::vector<int> m_0 (1, 1);
    ASSERT_EQ(*h_0, m_0);

    std::unique_ptr<std::vector<int> > h_1 = hadamard<std::vector<int> >(1);
    std::vector<int> m_1 (4, 1);
    m_1[3] = -1;
    ASSERT_EQ(*h_1, m_1);

    std::unique_ptr<std::vector<int> > h_2 = hadamard<std::vector<int> >(2);
    std::vector<int> m_2 (16, 1);
    m_2[5] = -1;
    m_2[7] = -1;
    m_2[10] = -1;
    m_2[11] = -1;
    m_2[13] = -1;
    m_2[14] = -1;
    ASSERT_EQ(*h_2, m_2);
}

} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
