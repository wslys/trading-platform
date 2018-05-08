/*
 Navicat MySQL Data Transfer

 Source Server         : Local-MySql
 Source Server Type    : MySQL
 Source Server Version : 50722
 Source Host           : localhost:3306
 Source Schema         : trading_platform

 Target Server Type    : MySQL
 Target Server Version : 50722
 File Encoding         : 65001

 Date: 08/05/2018 18:44:06
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for currencys
-- ----------------------------
DROP TABLE IF EXISTS `currencys`;
CREATE TABLE `currencys`  (
  `id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  `tp_id` int(11) NULL DEFAULT NULL,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_croatian_ci NULL DEFAULT NULL,
  `pair` varchar(255) CHARACTER SET utf8 COLLATE utf8_croatian_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 520 CHARACTER SET = utf8 COLLATE = utf8_croatian_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for real_time_data
-- ----------------------------
DROP TABLE IF EXISTS `real_time_data`;
CREATE TABLE `real_time_data`  (
  `id` int(11) NOT NULL,
  `tp_id` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_croatian_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for trading_platform
-- ----------------------------
DROP TABLE IF EXISTS `trading_platform`;
CREATE TABLE `trading_platform`  (
  `id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  `tage` varchar(255) CHARACTER SET utf8 COLLATE utf8_croatian_ci NULL DEFAULT NULL,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_croatian_ci NULL DEFAULT NULL,
  `url` varchar(255) CHARACTER SET utf8 COLLATE utf8_croatian_ci NULL DEFAULT NULL,
  `create_at` datetime(0) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_croatian_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
