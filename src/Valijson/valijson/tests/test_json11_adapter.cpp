#include <gtest/gtest.h>

#include <valijson/adapters/json11_adapter.hpp>

class TestJson11Adapter : public testing::Test
{

};

TEST_F(TestJson11Adapter, BasicArrayIteration)
{
    const unsigned int numElements = 10;

    // Create a Json11 document that consists of an array of numbers
    json11::Json::array array;
    for (unsigned int i = 0; i < numElements; i++) {
        json11::Json value(static_cast<double>(i));
        array.push_back(value);
    }
    json11::Json document(array);

    // Ensure that wrapping the document preserves the array and does not allow
    // it to be cast to other types
    valijson::adapters::Json11Adapter adapter(document);
    ASSERT_NO_THROW( adapter.getArray() );
    ASSERT_ANY_THROW( adapter.getBool() );
    ASSERT_ANY_THROW( adapter.getDouble() );
    ASSERT_ANY_THROW( adapter.getObject() );
    ASSERT_ANY_THROW( adapter.getString() );

    // Ensure that the array contains the expected number of elements
    EXPECT_EQ( numElements, adapter.getArray().size() );

    // Ensure that the elements are returned in the order they were inserted
    unsigned int expectedValue = 0;
    for (const valijson::adapters::Json11Adapter value : adapter.getArray()) {
        ASSERT_TRUE( value.isNumber() );
        EXPECT_EQ( double(expectedValue), value.getDouble() );
        expectedValue++;
    }

    // Ensure that the correct number of elements were iterated over
    EXPECT_EQ(numElements, expectedValue);
}

TEST_F(TestJson11Adapter, BasicObjectIteration)
{
    const unsigned int numElements = 10;

    // Create a DropBoxJson11 document that consists of an object that maps numeric
    // strings their corresponding numeric values
    json11::Json::object object;
    for (unsigned int i = 0; i < numElements; i++) {
        std::string name(std::to_string(i));
        object[name] = json11::Json(static_cast<double>(i));
    }
    json11::Json document(object);

    // Ensure that wrapping the document preserves the object and does not
    // allow it to be cast to other types
    valijson::adapters::Json11Adapter adapter(document);
    ASSERT_NO_THROW( adapter.getObject() );
    ASSERT_ANY_THROW( adapter.getArray() );
    ASSERT_ANY_THROW( adapter.getBool() );
    ASSERT_ANY_THROW( adapter.getDouble() );
    ASSERT_ANY_THROW( adapter.getString() );

    // Ensure that the object contains the expected number of members
    EXPECT_EQ( numElements, adapter.getObject().size() );

    // Ensure that the members are returned in the order they were inserted
    unsigned int expectedValue = 0;
    for (const valijson::adapters::Json11Adapter::ObjectMember member : adapter.getObject()) {
        ASSERT_TRUE( member.second.isNumber() );
        EXPECT_EQ( std::to_string(expectedValue), member.first );
        EXPECT_EQ( double(expectedValue), member.second.getDouble() );
        expectedValue++;
    }

    // Ensure that the correct number of elements were iterated over
    EXPECT_EQ( numElements, expectedValue );
}
