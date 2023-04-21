#include <unity.h>
#include "sensorcollection.h"

// #include "sensors.h"
// #include "bme680.h"
// #include "tsl2591.h"

void setUp(void) {}           // before test
void tearDown(void) {}        // after test

void test_sensorCollection_initalize() {
    sensorCollection aCollection;
    TEST_ASSERT_EQUAL_UINT32(0, aCollection.actualNumberOfSensors);
}

void test_sensorCollection_addSensor() {
    sensorCollection aCollection;
    aCollection.addSensor(measurementChannel::batteryLevel, 8, 360, 8, 15);
    TEST_ASSERT_EQUAL_UINT32(1, aCollection.actualNumberOfSensors);

    TEST_ASSERT_EQUAL_UINT32(8, aCollection.theSensorCollection[0].oversamplingLowPower);
    TEST_ASSERT_EQUAL_UINT32(360, aCollection.theSensorCollection[0].prescalerLowPower);
    TEST_ASSERT_EQUAL_UINT32(8, aCollection.theSensorCollection[0].oversamplingHighPower);
    TEST_ASSERT_EQUAL_UINT32(15, aCollection.theSensorCollection[0].prescalerHighPower);
}

void test_sensorCollection_addTooManySensors() {
    sensorCollection aCollection;
    for (uint32_t index = 0; index < (sensorCollection::maxNumberOfSensors + 2); index++) {
        aCollection.addSensor(measurementChannel::batteryLevel, 8, 360, 8, 15);
    }
    TEST_ASSERT_EQUAL_UINT32(sensorCollection::maxNumberOfSensors, aCollection.actualNumberOfSensors);
}

void test_sensorCollection_discover() {
    sensorCollection aCollection;
    aCollection.discover();
    TEST_ASSERT_EQUAL_UINT32(1, aCollection.actualNumberOfSensors);        // battery is always present, all other sensors are not being detected as I2C is not being mocked
}

void test_sensor_initialization() {
    sensor aSensor;
    TEST_ASSERT_EQUAL(measurementChannel::none, aSensor.type);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.oversamplingLowPower);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.prescalerLowPower);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.oversamplingHighPower);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.prescalerHighPower);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.oversamplingCounter);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.prescaleCounter);
    for (uint32_t index = 0; index < sensor::maxOversampling; index++) {
        TEST_ASSERT_EQUAL_FLOAT(0.0F, aSensor.sample[index]);
    }
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_sensorCollection_initalize);
    RUN_TEST(test_sensorCollection_addSensor);
    RUN_TEST(test_sensorCollection_addTooManySensors);
    RUN_TEST(test_sensorCollection_discover);
    RUN_TEST(test_sensor_initialization);
    UNITY_END();
}
