# OpenXR Spatial Anchors Sample

## Overview
Spatial entities are application-defined content that persist in real-world locations.

* `XR_FB_spatial_entity`: This extension enables applications to persist the real-world location of content over time and contains definitions for the Entity-Component System. All Facebook spatial entities and scene extensions are dependent on this one.
* `XR_FB_spatial_entity_query`: This extension enables an application to discover persistent spatial entities in the area and restore them.
* `XR_FB_spatial_entity_storage`: This extension enables spatial entities to be stored and persisted across sessions.
* `XR_FB_spatial_entity_storage_batch`: This extension enables multiple spatial entities to be persisted across sessions at a time.
* `XR_FB_spatial_entity_sharing`: This extension enables spatial entities to be shared between users.
* `XR_FB_spatial_entity_user`: This extension enables the creation and management of user objects which can be used by the application to reference a user other than the current user.

## The Sample
In this sample, the orange cuboid represents Spatial Anchors. The user is able to place an anchor in the space using the 'A' button at the controller's position. The user can remove the previously placed anchor using the 'B' button. The user can refresh anchors by querying all anchors using the 'X' button. The user can share all loaded anchors using the 'Y' button.
