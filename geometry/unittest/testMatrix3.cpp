#include <UnitTest.hpp>
#include <geometry/Matrix3.hpp>
#include <geometry/Quat.hpp>

using namespace geo;

RUN_UNIT_TEST(TestMatrix3)
{
    Matrix3f mat3f1(1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f);

    ASSERT(!mat3f1.isIdentity());

    Matrix3f mat3f2;
    mat3f2.makeIdentity();

    ASSERT(mat3f1 != mat3f2);
    ASSERT(!mat3f1.equals(mat3f2));

    mat3f1.inverse();

	Quatf rot;
	mat3f1.setRotate(rot);
}