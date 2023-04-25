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

void test_sensor_run_inactive() {
    sensor aSensor;
    aSensor.type = measurementChannel::batteryLevel;
    for (uint32_t index = 0; index < 20; index++) {
        aSensor.run();
    }
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.prescaleCounter);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.oversamplingCounter);
}

void test_sensor_run_active_prescaling() {
    const uint32_t prescalerTestValue{3};
    sensor aSensor;
    aSensor.type                 = measurementChannel::batteryLevel;
    aSensor.prescalerLowPower    = prescalerTestValue;        // this makes us take a sample every (preprescalerTestValuescaler + 1) runs
    aSensor.oversamplingLowPower = 0;
    aSensor.active               = true;

    aSensor.run();
    TEST_ASSERT_EQUAL_UINT32(aSensor.prescalerLowPower, aSensor.prescaleCounter);
    TEST_ASSERT_EQUAL_UINT32(aSensor.oversamplingLowPower, aSensor.oversamplingCounter);

    for (uint32_t index = 0; index < (prescalerTestValue); index++) {
        aSensor.run();
    }
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.prescaleCounter);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.oversamplingCounter);
    TEST_ASSERT_EQUAL_FLOAT(3.3F, aSensor.sample[0]);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, aSensor.sample[1]);
}

void test_sensor_run_active_oversampling() {
    const uint32_t oversamplingTestValue{3};
    sensor aSensor;
    aSensor.type                 = measurementChannel::batteryLevel;
    aSensor.prescalerLowPower    = 0;                            // this makes us take a sample every run
    aSensor.oversamplingLowPower = oversamplingTestValue;        // this makes us average (oversamplingTestValue + 1) samples into a measurement
    aSensor.active               = true;

    aSensor.run();
    TEST_ASSERT_EQUAL_UINT32(aSensor.prescalerLowPower, aSensor.prescaleCounter);
    TEST_ASSERT_EQUAL_UINT32(aSensor.oversamplingLowPower, aSensor.oversamplingCounter);

    for (uint32_t index = 0; index < (oversamplingTestValue); index++) {
        aSensor.run();
    }
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.prescaleCounter);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.oversamplingCounter);

    for (uint32_t index = 0; index < (oversamplingTestValue + 1); index++) {
        TEST_ASSERT_EQUAL_FLOAT(3.3F, aSensor.sample[0]);
    }
}

void test_sensor_run_active_prescaling_and_oversampling() {
    const uint32_t prescalerTestValue{3};
    const uint32_t oversamplingTestValue{3};
    sensor aSensor;
    aSensor.type                 = measurementChannel::batteryLevel;
    aSensor.prescalerLowPower    = prescalerTestValue;           // this makes us take a sample every (prescalerTestValue + 1) runs
    aSensor.oversamplingLowPower = oversamplingTestValue;        // this makes us average (oversamplingTestValue + 1) samples into a measurement
    aSensor.active               = true;

    aSensor.run();
    TEST_ASSERT_EQUAL_UINT32(aSensor.prescalerLowPower, aSensor.prescaleCounter);
    TEST_ASSERT_EQUAL_UINT32(aSensor.oversamplingLowPower, aSensor.oversamplingCounter);

    for (uint32_t index = 0; index < (((oversamplingTestValue + 1) * (prescalerTestValue + 1)) - 1); index++) {
        aSensor.run();
    }
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.prescaleCounter);
    TEST_ASSERT_EQUAL_UINT32(0, aSensor.oversamplingCounter);

    for (uint32_t index = 0; index < (oversamplingTestValue + 1); index++) {
        TEST_ASSERT_EQUAL_FLOAT(3.3F, aSensor.sample[0]);
    }
}

void test_sensor_transition_high_low_power() {
    const uint32_t prescalerTestValue{3};
    const uint32_t oversamplingTestValue{3};
    sensor aSensor;
    aSensor.type                 = measurementChannel::batteryLevel;
    aSensor.prescalerLowPower    = prescalerTestValue;
    aSensor.oversamplingLowPower = oversamplingTestValue;
    aSensor.prescaleCounter      = sensor::maxPrescaler + 1;
    aSensor.oversamplingCounter  = sensor::maxOversampling + 1;
    aSensor.active               = true;
    aSensor.run();
    TEST_ASSERT_EQUAL_UINT32(prescalerTestValue - 1, aSensor.prescaleCounter);
    TEST_ASSERT_EQUAL_UINT32(aSensor.oversamplingLowPower, aSensor.oversamplingCounter);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_sensorCollection_initalize);
    RUN_TEST(test_sensorCollection_addSensor);
    RUN_TEST(test_sensorCollection_addTooManySensors);
    RUN_TEST(test_sensorCollection_discover);
    RUN_TEST(test_sensor_initialization);
    RUN_TEST(test_sensor_run_inactive);
    RUN_TEST(test_sensor_run_active_prescaling);
    RUN_TEST(test_sensor_run_active_oversampling);
    RUN_TEST(test_sensor_run_active_prescaling_and_oversampling);
    RUN_TEST(test_sensor_transition_high_low_power);
    UNITY_END();
}
