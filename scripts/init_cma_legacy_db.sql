/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50615
Source Host           : localhost:3306
Source Database       : cma_legacy

Target Server Type    : MYSQL
Target Server Version : 50615
File Encoding         : 65001

Date: 2014-03-06 10:34:05
*/

CREATE DATABASE cma_legacy;
USE cma_legacy;

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for cma_legacy_device
-- ----------------------------
DROP TABLE IF EXISTS `cma_legacy_device`;
CREATE TABLE `cma_legacy_device` (
  `OBJID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Netbasestation_ID` int(5) unsigned NOT NULL default 0,
  `Voltage_Level` varchar(10) NOT NULL default '',
  `SN` char(11) NOT NULL default '',
  `Company` varchar(30) NOT NULL default '',
  `Contract` varchar(30) NOT NULL default '',
  `ServerInfo` text NOT NULL default '',
  `Line` varchar(30) NOT NULL default '',
  `Tower_ID` varchar(8) NOT NULL default '',
  `Location` varchar(20) NOT NULL default '',
  `SIM` char(11) NOT NULL default '',
  `DeviceType_ID` tinyint(4) NOT NULL default 0,
  `ServerIP` varchar(25) NOT NULL default '',
  `DeviceIP` varchar(25) NOT NULL default '',
  UNIQUE(`Netbasestation_ID`),
  PRIMARY KEY (`OBJID`),
  KEY `DeviceType_ID` (`DeviceType_ID`),
  CONSTRAINT `cma_legacy_device_ibfk_1` FOREIGN KEY (`DeviceType_ID`) REFERENCES `cma_legacy_devtypes` (`TypeID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cma_legacy_device
-- ----------------------------

-- ----------------------------
-- Table structure for cma_legacy_devtypes
-- ----------------------------
DROP TABLE IF EXISTS `cma_legacy_devtypes`;
CREATE TABLE `cma_legacy_devtypes` (
  `OBJID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `TypeName` varchar(10) NOT NULL default '',
  `TypeCommnet` varchar(10) NOT NULL default '',
  `TypeID` tinyint(4) NOT NULL default 0,
  `DataTableName` varchar(20) NOT NULL default '',
  PRIMARY KEY (`OBJID`),
  KEY `TypeID` (`TypeID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for cma_legacy_dxwdjt
-- ----------------------------
DROP TABLE IF EXISTS `cma_legacy_dxwdjt`;
CREATE TABLE `cma_legacy_dxwdjt` (
  `OBJID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Sensor_ID` int(10) NOT NULL default 0,
  `Timstamp` datetime NOT NULL default 0,
  `WindSpeed` decimal(10,0) NOT NULL,
  `Wind_Direction` decimal(10,0) NOT NULL,
  `Air_Temperature` decimal(10,0) NOT NULL,
  `Humidity` decimal(10,0) NOT NULL,
  `Air_Pressure` decimal(10,0) NOT NULL,
  `Precipitation_Intensity` decimal(10,0) NOT NULL,
  `Radiation_Intensity` decimal(10,0) NOT NULL,
  `Daily_Rainfall` decimal(10,0) NOT NULL,
  `Battery_Voltage` decimal(10,0) NOT NULL,
  `Operation_Temperature` decimal(10,0) NOT NULL,
  `Netbasestation_ID` int(5) NOT NULL,
  PRIMARY KEY (`OBJID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cma_legacy_dxwdjt
-- ----------------------------

-- ----------------------------
-- Table structure for cma_legacy_qxhjjt
-- ----------------------------
DROP TABLE IF EXISTS `cma_legacy_qxhjjt`;
CREATE TABLE `cma_legacy_qxhjjt` (
  `OBJID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Sensor_ID` int(10) NOT NULL,
  `Timstamp` datetime NOT NULL,
  `WindSpeed` decimal(10,0) NOT NULL,
  `Wind_Direction` decimal(10,0) NOT NULL,
  `Air_Temperature` decimal(10,0) NOT NULL,
  `Humidity` decimal(10,0) NOT NULL,
  `Air_Pressure` decimal(10,0) NOT NULL,
  `Precipitation_Intensity` decimal(10,0) NOT NULL,
  `Radiation_Intensity` decimal(10,0) NOT NULL,
  `Daily_Rainfall` decimal(10,0) NOT NULL,
  `Battery_Voltage` decimal(10,0) NOT NULL,
  `Operation_Temperature` decimal(10,0) NOT NULL,
  `Netbasestation_ID` int(5) NOT NULL,
  PRIMARY KEY (`OBJID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cma_legacy_qxhjjt
-- ----------------------------
