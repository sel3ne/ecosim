#include "delivery.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unordered_set>

#include "building.h"
#include "entity.h"
#include "human.h"
#include "map_generator.h"
#include "settings.h"
#include "world.h"

class DeliveryTest : public ::testing::Test {
 protected:
  void SetUp() override {
    world_ = std::make_unique<World>(std::make_unique<SimpleIslandGenerator>());

    src_building_ = world_->buildConstructible<Building>(
        /*x_grid=*/0, /*y_grid=*/0, /*w_grid=*/1, /*h_grid=*/1,
        Entity::LIGHTHOUSE);
    dest_building_ = world_->buildConstructible<Building>(
        /*x_grid=*/1, /*y_grid=*/0, /*w_grid=*/1, /*h_grid=*/1,
        Entity::LIGHTHOUSE);

    src_building_->addDeliveryTarget(RESOURCE_TEST, dest_building_);
  }

  std::unique_ptr<World> world_;
  Building* src_building_;
  Building* dest_building_;
};

TEST_F(DeliveryTest, DeliveryIsCreatedWhenResourceBecomesAvailable) {
  dest_building_->setRequiredResourceAmount(RESOURCE_TEST,
                                            kCarrierCapacity * 2);
  // Nothing should be incoming so far.
  ASSERT_EQ(dest_building_->returnIncomingResourceAmount(RESOURCE_TEST), 0);

  // Add not enough for a delivery.
  src_building_->setAvailableResourceAmount(RESOURCE_TEST,
                                            kCarrierCapacity - 1);
  EXPECT_EQ(dest_building_->returnIncomingResourceAmount(RESOURCE_TEST), 0);

  // Adding more should automatically create a single delivery.
  src_building_->addToAvailableResourceAmount(RESOURCE_TEST, 2);
  EXPECT_EQ(dest_building_->returnIncomingResourceAmount(RESOURCE_TEST),
            kCarrierCapacity);

  // There should now be a delivery.
  Delivery* delivery = world_->getOldestUnassignedDelivery();
  ASSERT_NE(delivery, nullptr);
  ASSERT_EQ(delivery->getSource(), src_building_);
  ASSERT_EQ(delivery->getDestination(), dest_building_);
  ASSERT_EQ(delivery->getCarrier(), nullptr);
  ASSERT_EQ(delivery->getResource(), RESOURCE_TEST);
}

TEST_F(DeliveryTest, DeliveryIsCreatedOnNewNeed) {
  src_building_->setAvailableResourceAmount(RESOURCE_TEST,
                                            kCarrierCapacity * 2);
  // Nothing should be incoming so far.
  ASSERT_EQ(dest_building_->returnIncomingResourceAmount(RESOURCE_TEST), 0);

  // Request too small amount to deliver.
  dest_building_->setRequiredResourceAmount(RESOURCE_TEST,
                                            kCarrierCapacity - 1);
  EXPECT_EQ(dest_building_->returnIncomingResourceAmount(RESOURCE_TEST), 0);

  // Requesting more should automatically create a single delivery.
  dest_building_->addToRequiredResourceAmount(RESOURCE_TEST, 2);
  EXPECT_EQ(dest_building_->returnIncomingResourceAmount(RESOURCE_TEST),
            kCarrierCapacity);

  // There should now be a delivery.
  Delivery* delivery = world_->getOldestUnassignedDelivery();
  ASSERT_NE(delivery, nullptr);
  ASSERT_EQ(delivery->getSource(), src_building_);
  ASSERT_EQ(delivery->getDestination(), dest_building_);
  ASSERT_EQ(delivery->getCarrier(), nullptr);
  ASSERT_EQ(delivery->getResource(), RESOURCE_TEST);
}

TEST_F(DeliveryTest, DeliveryIsAssignedOnCreation) {
  // Make 2 humans to carry things.
  for (int i = 0; i < 2; ++i) {
    world_->addEntityToEntities(std::make_unique<Human>(
        /*x_world=*/0., /*y_world=*/0., /*w_world=*/8,
        /*h_world=*/8, Entity::HUMAN));
  }

  src_building_->setAvailableResourceAmount(RESOURCE_TEST,
                                            kCarrierCapacity * 3);
  dest_building_->setRequiredResourceAmount(RESOURCE_TEST,
                                            kCarrierCapacity * 3);

  // There should now be two deliveries assigned and one unassigned.
  std::unordered_set<Human*> carriers;
  for (const std::unique_ptr<Delivery>& delivery : world_->returnDeliveries()) {
    carriers.insert(delivery->getCarrier());
  }

  // There should be three values: The two humans and nullptr for the third.
  EXPECT_THAT(carriers, testing::SizeIs(3));
}

TEST_F(DeliveryTest, DeliveryIsAssignedOnHumanBecomingUnemployed) {
  std::unique_ptr<Human> owned_human = std::make_unique<Human>(
      /*x_world=*/0., /*y_world=*/0., /*w_world=*/8,
      /*h_world=*/8, Entity::HUMAN);
  Human* human = owned_human.get();
  world_->addEntityToEntities(std::move(owned_human));

  // Let's pretend this human is already employed.
  human->setJob(Human::CARRIER);

  // Create a Delivery.
  src_building_->setAvailableResourceAmount(RESOURCE_TEST, kCarrierCapacity);
  dest_building_->setRequiredResourceAmount(RESOURCE_TEST, kCarrierCapacity);

  // Find the just created delivery.
  ASSERT_THAT(world_->returnDeliveries(), testing::SizeIs(1));
  Delivery* delivery = world_->returnDeliveries().front().get();

  // There shouldn't be a carrier yet.
  ASSERT_EQ(delivery->getCarrier(), nullptr);

  // Making the human unemployed should make him pick up the delivery.
  human->setJob(Human::UNEMPLOYED);
  ASSERT_EQ(delivery->getCarrier(), human);
  ASSERT_EQ(human->returnJob(), Human::CARRIER);
}
