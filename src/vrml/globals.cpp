/*
 * globals.cpp
 *
 *  Created on: Aug 18, 2015
 *      Author: nbingham
 */

#include "globals.h"

namespace parse_vrml
{

addr::addr()
{

}

addr::addr(string kind, string name)
{
	this->kind = kind;
	this->name = name;
}

addr::~addr()
{

}

bool operator<(addr a0, addr a1)
{
	return (a0.kind < a1.kind) ||
		   (a0.kind == a1.kind && a0.name < a1.name);
}

bool operator>(addr a0, addr a1)
{
	return (a0.kind > a1.kind) ||
		   (a0.kind == a1.kind && a0.name > a1.name);
}

bool operator<=(addr a0, addr a1)
{
	return (a0.kind < a1.kind) ||
		   (a0.kind == a1.kind && a0.name <= a1.name);
}

bool operator>=(addr a0, addr a1)
{
	return (a0.kind > a1.kind) ||
		   (a0.kind == a1.kind && a0.name >= a1.name);
}

bool operator==(addr a0, addr a1)
{
	return (a0.kind == a1.kind && a0.name == a1.name);
}

bool operator!=(addr a0, addr a1)
{
	return (a0.kind != a1.kind || a0.name != a1.name);
}

field::field()
{

}

field::field(string kind, string type)
{
	this->kind = kind;
	this->type = type;
}

field::~field()
{

}

globals::globals()
{
	// Grouping Nodes

	// Anchor
	fields.insert(pair<string, field>("Anchor.addChildren",			field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Anchor.removeChildren",		field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Anchor.children",			field("exposedField",	"MFNode")));
	fields.insert(pair<string, field>("Anchor.description",			field("exposedField",	"SFString")));
	fields.insert(pair<string, field>("Anchor.parameter",			field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Anchor.url",					field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Anchor.bboxCenter",			field("field",			"SFVec3f")));
	fields.insert(pair<string, field>("Anchor.bboxSize",			field("field",			"SFVec3f")));

	// Billboard
	fields.insert(pair<string, field>("Billboard.addChildren",		field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Billboard.removeChildren",	field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Billboard.axisOfRotation",	field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("Billboard.children",			field("exposedField",	"MFNode")));
	fields.insert(pair<string, field>("Billboard.bboxCenter",		field("field",			"SFVec3f")));
	fields.insert(pair<string, field>("Billboard.bboxSize",			field("field",			"SFVec3f")));

	// Collision
	fields.insert(pair<string, field>("Collision.addChildren",		field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Collision.removeChildren",	field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Collision.children",			field("exposedField",	"MFNode")));
	fields.insert(pair<string, field>("Collision.collide",			field("exposedField",	"SFBool")));
	fields.insert(pair<string, field>("Collision.bboxCenter",		field("field",			"SFVec3f")));
	fields.insert(pair<string, field>("Collision.bboxSize",			field("field",			"SFVec3f")));
	fields.insert(pair<string, field>("Collision.proxy",			field("field",			"SFNode")));
	fields.insert(pair<string, field>("Collision.collideTime",		field("eventOut",		"SFTime")));

	// Group
	fields.insert(pair<string, field>("Group.addChildren",			field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Group.removeChildren",		field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Group.children",				field("exposedField",	"MFNode")));
	fields.insert(pair<string, field>("Group.bboxCenter",			field("field",			"SFVec3f")));
	fields.insert(pair<string, field>("Group.bboxSize",				field("field",			"SFVec3f")));

	// Transform
	fields.insert(pair<string, field>("Transform.addChildren",		field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Transform.removeChildren",	field("eventIn",		"MFNode")));
	fields.insert(pair<string, field>("Transform.center",			field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("Transform.children",			field("exposedField",	"MFNode")));
	fields.insert(pair<string, field>("Transform.rotation",			field("exposedField",	"SFRotation")));
	fields.insert(pair<string, field>("Transform.scale",			field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("Transform.scaleOrientation",	field("exposedField",	"SFRotation")));
	fields.insert(pair<string, field>("Transform.translation",		field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("Transform.bboxCenter",		field("field",			"SFVec3f")));
	fields.insert(pair<string, field>("Transform.bboxSize",			field("field",			"SFVec3f")));

	// Special Groups

	// Inline
	fields.insert(pair<string, field>("Inline.url",					field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Inline.bboxCenter",			field("field",			"SFVec3f")));
	fields.insert(pair<string, field>("Inline.bboxSize",			field("field",			"SFVec3f")));

	// LOD
	fields.insert(pair<string, field>("LOD.level",					field("exposedField",	"MFNode")));
	fields.insert(pair<string, field>("LOD.center",					field("field",			"SFVec3f")));
	fields.insert(pair<string, field>("LOD.range",					field("field",			"MFFloat")));

	// Switch
	fields.insert(pair<string, field>("Switch.choice",				field("exposedField",	"MFNode")));
	fields.insert(pair<string, field>("Switch.whichChoice",			field("exposedField",	"SFInt32")));

	// Common Nodes

	// AudioClip
	fields.insert(pair<string, field>("AudioClip.description",		field("exposedField",	"SFString")));
	fields.insert(pair<string, field>("AudioClip.loop",				field("exposedField",	"SFBool")));
	fields.insert(pair<string, field>("AudioClip.pitch",			field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("AudioClip.startTime",		field("exposedField",	"SFTime")));
	fields.insert(pair<string, field>("AudioClip.stopTime",			field("exposedField",	"SFTime")));
	fields.insert(pair<string, field>("AudioClip.url",				field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("AudioClip.duration_changed",	field("eventOut",		"SFTime")));
	fields.insert(pair<string, field>("AudioClip.isActive",			field("eventOut",		"SFBool")));

	// DirectionalLight
	fields.insert(pair<string, field>("DirectionalLight.ambientIntensity",	field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("DirectionalLight.color",				field("exposedField",	"SFColor")));
	fields.insert(pair<string, field>("DirectionalLight.direction",			field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("DirectionalLight.intensity",			field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("DirectionalLight.on",				field("exposedField",	"SFBool")));

	// PointLight
	fields.insert(pair<string, field>("PointLight.ambientIntensity",	field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("PointLight.attenuation",			field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("PointLight.color",				field("exposedField",	"SFColor")));
	fields.insert(pair<string, field>("PointLight.intensity",			field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("PointLight.location",			field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("PointLight.on",					field("exposedField",	"SFBool")));
	fields.insert(pair<string, field>("PointLight.radius",				field("exposedField",	"SFFloat")));

	// Script
	// TODO

	// Shape
	fields.insert(pair<string, field>("Shape.appearance",			field("exposedField",	"SFNode")));
	fields.insert(pair<string, field>("Shape.geometry",				field("exposedField",	"SFNode")));

	// Sound
	fields.insert(pair<string, field>("Sound.direction",			field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("Sound.intensity",			field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Sound.location",				field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("Sound.maxBack",				field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Sound.maxFront",				field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Sound.minBack",				field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Sound.minFront",				field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Sound.priority",				field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Sound.source",				field("exposedField",	"SFNode")));
	fields.insert(pair<string, field>("Sound.spatialize",			field("field",			"SFBool")));

	// SpotLight
	fields.insert(pair<string, field>("SpotLight.ambientIntensity",	field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("SpotLight.attenuation",		field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("SpotLight.beamWidth",		field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("SpotLight.color",			field("exposedField",	"SFColor")));
	fields.insert(pair<string, field>("SpotLight.cutOffAngle",		field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("SpotLight.direction",		field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("SpotLight.intensity",		field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("SpotLight.location",			field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("SpotLight.on",				field("exposedField",	"SFBool")));
	fields.insert(pair<string, field>("SpotLight.radius",			field("exposedField",	"SFFloat")));

	// WorldInfo
	fields.insert(pair<string, field>("WorldInfo.info",				field("field",			"MFString")));
	fields.insert(pair<string, field>("WorldInfo.title",			field("field",			"SFString")));

	// Sensors

	// CylinderSensor
	// TODO

	// PlaneSensor
	// ProximitySensor
	// SphereSensor
	// TimeSensor
	fields.insert(pair<string, field>("TimeSensor.cycleInterval",		field("exposedField",	"SFTime")));
	fields.insert(pair<string, field>("TimeSensor.enabled",				field("exposedField",	"SFBool")));
	fields.insert(pair<string, field>("TimeSensor.loop",				field("exposedField",	"SFBool")));
	fields.insert(pair<string, field>("TimeSensor.startTime",			field("exposedField",	"SFTime")));
	fields.insert(pair<string, field>("TimeSensor.stopTime",			field("exposedField",	"SFTime")));
	fields.insert(pair<string, field>("TimeSensor.cycleTime",			field("eventOut",		"SFTime")));
	fields.insert(pair<string, field>("TimeSensor.fraction_changed",	field("eventOut",		"SFFloat")));
	fields.insert(pair<string, field>("TimeSensor.isActive",			field("eventOut",		"SFBool")));
	fields.insert(pair<string, field>("TimeSensor.time",				field("eventOut",		"SFTime")));

	// TouchSensor
	// VisibilitySensor

	// Geometry

	// Box
	fields.insert(pair<string, field>("Box.size",		field("field",	"SFVec3f")));

	// Cone
	fields.insert(pair<string, field>("Cone.bottomRadius",	field("field",	"SFFloat")));
	fields.insert(pair<string, field>("Cone.height",		field("field",	"SFFloat")));
	fields.insert(pair<string, field>("Cone.side",			field("field",	"SFBool")));
	fields.insert(pair<string, field>("Cone.bottom",		field("field",	"SFBool")));

	// Cylinder
	fields.insert(pair<string, field>("Cylinder.bottom",	field("field",	"SFBool")));
	fields.insert(pair<string, field>("Cylinder.height",	field("field",	"SFFloat")));
	fields.insert(pair<string, field>("Cylinder.radius",	field("field",	"SFFloat")));
	fields.insert(pair<string, field>("Cylinder.side",		field("field",	"SFBool")));
	fields.insert(pair<string, field>("Cylinder.top",		field("field",	"SFBool")));

	// ElevationGrid
	// Extrusion
	// IndexedFaceSet
	fields.insert(pair<string, field>("IndexedFaceSet.set_colorIndex",			field("eventIn",		"MFInt32")));
	fields.insert(pair<string, field>("IndexedFaceSet.set_coordIndex",			field("eventIn",		"MFInt32")));
	fields.insert(pair<string, field>("IndexedFaceSet.set_normalIndex",			field("eventIn",		"MFInt32")));
	fields.insert(pair<string, field>("IndexedFaceSet.set_texCoordIndex",		field("eventIn",		"MFInt32")));
	fields.insert(pair<string, field>("IndexedFaceSet.color",					field("exposedField",	"SFNode")));
	fields.insert(pair<string, field>("IndexedFaceSet.coord",					field("exposedField",	"SFNode")));
	fields.insert(pair<string, field>("IndexedFaceSet.normal",					field("exposedField",	"SFNode")));
	fields.insert(pair<string, field>("IndexedFaceSet.texCoord",				field("exposedField",	"SFNode")));
	fields.insert(pair<string, field>("IndexedFaceSet.ccw",						field("field",			"SFBool")));
	fields.insert(pair<string, field>("IndexedFaceSet.colorIndex",				field("field",			"MFInt32")));
	fields.insert(pair<string, field>("IndexedFaceSet.colorPerVertex",			field("field",			"SFBool")));
	fields.insert(pair<string, field>("IndexedFaceSet.convex",					field("field",			"SFBool")));
	fields.insert(pair<string, field>("IndexedFaceSet.coordIndex",				field("field",			"MFInt32")));
	fields.insert(pair<string, field>("IndexedFaceSet.creaseAngle",				field("field",			"SFFloat")));
	fields.insert(pair<string, field>("IndexedFaceSet.normalIndex",				field("field",			"MFInt32")));
	fields.insert(pair<string, field>("IndexedFaceSet.normalPerVertex",			field("field",			"SFBool")));
	fields.insert(pair<string, field>("IndexedFaceSet.solid",					field("field",			"SFBool")));
	fields.insert(pair<string, field>("IndexedFaceSet.texCoordIndex",			field("field",			"MFInt32")));

	// IndexedLineSet
	// PointSet
	// Sphere
	fields.insert(pair<string, field>("Sphere.radius",		field("field",	"SFFloat")));

	// Text

	// Geometric Properties

	// Color
	fields.insert(pair<string, field>("Color.color",				field("exposedField",	"MFColor")));

	// Coordinate
	fields.insert(pair<string, field>("Coordinate.point",			field("exposedField",	"MFVec3f")));

	// Normal
	fields.insert(pair<string, field>("Normal.vector",				field("exposedField",	"MFVec3f")));

	// TextureCoordinate
	fields.insert(pair<string, field>("TextureCoordinate.point",	field("exposedField",	"MFVec2f")));

	// Appearance

	// Appearance
	fields.insert(pair<string, field>("Appearance.material",			field("exposedField",	"SFNode")));
	fields.insert(pair<string, field>("Appearance.texture",				field("exposedField",	"SFNode")));
	fields.insert(pair<string, field>("Appearance.textureTransform",	field("exposedField",	"SFNode")));

	// FontStyle
	fields.insert(pair<string, field>("FontStyle.family",				field("field",	"SFString")));
	fields.insert(pair<string, field>("FontStyle.horizontal",			field("field",	"SFBool")));
	fields.insert(pair<string, field>("FontStyle.justify",				field("field",	"MFString")));
	fields.insert(pair<string, field>("FontStyle.language",				field("field",	"SFString")));
	fields.insert(pair<string, field>("FontStyle.leftToRight",			field("field",	"SFBool")));
	fields.insert(pair<string, field>("FontStyle.size",					field("field",	"SFFloat")));
	fields.insert(pair<string, field>("FontStyle.spacing",				field("field",	"SFFloat")));
	fields.insert(pair<string, field>("FontStyle.style",				field("field",	"SFString")));
	fields.insert(pair<string, field>("FontStyle.topToBottom",			field("field",	"SFBool")));

	// ImageTexture
	fields.insert(pair<string, field>("ImageTexture.url",				field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("ImageTexture.repeatS",			field("field",			"SFBool")));
	fields.insert(pair<string, field>("ImageTexture.repeatT",			field("field",			"SFBool")));

	// Material
	fields.insert(pair<string, field>("Material.ambientIntensity",			field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Material.diffuseColor",				field("exposedField",	"SFColor")));
	fields.insert(pair<string, field>("Material.emissiveColor",				field("exposedField",	"SFColor")));
	fields.insert(pair<string, field>("Material.shininess",					field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Material.specularColor",				field("exposedField",	"SFColor")));
	fields.insert(pair<string, field>("Material.transparency",				field("exposedField",	"SFFloat")));

	// MovieTexture
	// PixelTexture
	// TextureTransform

	// Interpolators

	// ColorInterpolator
	// CoordinateInterpolator
	// NormalInterpolator
	// OrientationInterpolator
	fields.insert(pair<string, field>("OrientationInterpolator.set_fraction",		field("eventIn",		"SFFloat")));
	fields.insert(pair<string, field>("OrientationInterpolator.key",				field("exposedField",	"MFFloat")));
	fields.insert(pair<string, field>("OrientationInterpolator.keyValue",			field("exposedField",	"MFRotation")));
	fields.insert(pair<string, field>("OrientationInterpolator.value_changed",		field("eventOut",		"SFRotation")));

	// PositionInterpolator
	fields.insert(pair<string, field>("PositionInterpolator.set_fraction",		field("eventIn",		"SFFloat")));
	fields.insert(pair<string, field>("PositionInterpolator.key",				field("exposedField",	"MFFloat")));
	fields.insert(pair<string, field>("PositionInterpolator.keyValue",			field("exposedField",	"MFVec3f")));
	fields.insert(pair<string, field>("PositionInterpolator.value_changed",		field("eventOut",		"SFVec3f")));

	// ScalarInterpolator

	// Bindable Nodes

	// Background
	fields.insert(pair<string, field>("Background.set_bind",		field("eventIn",		"SFBool")));
	fields.insert(pair<string, field>("Background.groundAngle",		field("exposedField",	"MFFloat")));
	fields.insert(pair<string, field>("Background.groundColor",		field("exposedField",	"MFColor")));
	fields.insert(pair<string, field>("Background.backUrl",			field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Background.bottomUrl",		field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Background.frontUrl",		field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Background.leftUrl",			field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Background.rightUrl",		field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Background.topUrl",			field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("Background.skyAngle",		field("exposedField",	"MFFloat")));
	fields.insert(pair<string, field>("Background.skyColor",		field("exposedField",	"MFColor")));
	fields.insert(pair<string, field>("Background.isBound",			field("eventOut",		"SFBool")));


	// Fog
	// NavigationInfo
	fields.insert(pair<string, field>("NavigationInfo.set_bind",		field("eventIn",		"SFBool")));
	fields.insert(pair<string, field>("NavigationInfo.avatarSize",		field("exposedField",	"MFFloat")));
	fields.insert(pair<string, field>("NavigationInfo.headlight",		field("exposedField",	"SFBool")));
	fields.insert(pair<string, field>("NavigationInfo.speed",			field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("NavigationInfo.type",			field("exposedField",	"MFString")));
	fields.insert(pair<string, field>("NavigationInfo.visibilityLimit",	field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("NavigationInfo.isBound",			field("eventOut",		"SFBool")));

	// Viewpoint
	fields.insert(pair<string, field>("Viewpoint.set_bind",				field("eventIn",		"SFBool")));
	fields.insert(pair<string, field>("Viewpoint.fieldOfView",			field("exposedField",	"SFFloat")));
	fields.insert(pair<string, field>("Viewpoint.jump",					field("exposedField",	"SFBool")));
	fields.insert(pair<string, field>("Viewpoint.orientation",			field("exposedField",	"SFRotation")));
	fields.insert(pair<string, field>("Viewpoint.position",				field("exposedField",	"SFVec3f")));
	fields.insert(pair<string, field>("Viewpoint.description",			field("field",			"SFString")));
	fields.insert(pair<string, field>("Viewpoint.bindTime",				field("eventOut",		"SFTime")));
	fields.insert(pair<string, field>("Viewpoint.isBound",				field("eventOut",		"SFBool")));
}

globals::~globals()
{

}

}
