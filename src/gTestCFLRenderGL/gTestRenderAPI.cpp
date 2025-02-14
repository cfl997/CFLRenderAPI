#include "gTestInclude.h"
#if 1


#include "gTestEnvironment.h"

extern testing::Environment* const g_env;

class gTestRenderAPI : public testing::Test
{
public:
	gTestRenderAPI()
	{
		gTestEnviroment* env = dynamic_cast<gTestEnviroment*>(g_env);
		contextid = env->glContextID;
	}
	~gTestRenderAPI()
	{

	}

	// Sets up the test fixture.
	virtual void SetUp() override
	{
		vertexes.push_back({ glm::vec3{ 0,0,1 }, (DWORD)0xffff00ff, glm::vec2{ 0,0 } });




		indexes.emplace_back(1);
	}

	// Tears down the test fixture.
	virtual void TearDown() override
	{

	}

protected:
	int contextid = -1;
	VertexArray vertexes;
	std::vector<uint32_t>indexes;

private:

};

TEST_F(gTestRenderAPI, CFLRenderGL_MakeCurrentandunMake)
{
	bool isMake = CFLRenderGL_MakeCurrent(contextid);
	EXPECT_EQ(isMake, true);
	bool isunMake = CFLRenderGL_UnMakeCurrent(contextid);
	EXPECT_EQ(isunMake, true);
}

#endif // 0
