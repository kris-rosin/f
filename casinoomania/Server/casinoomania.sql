-- phpMyAdmin SQL Dump
-- version 4.6.5.2
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Jul 24, 2017 at 11:47 AM
-- Server version: 5.6.35
-- PHP Version: 5.5.38

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `casinoomania`
--

-- --------------------------------------------------------

--
-- Table structure for table `all_time_leaderboard`
--

CREATE TABLE `all_time_leaderboard` (
  `id` int(11) NOT NULL,
  `uid` int(11) NOT NULL,
  `gid` int(11) NOT NULL,
  `bet` bigint(20) NOT NULL,
  `win` bigint(20) NOT NULL,
  `diff` bigint(20) NOT NULL,
  `time_updated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `all_time_leaderboard`
--

INSERT INTO `all_time_leaderboard` (`id`, `uid`, `gid`, `bet`, `win`, `diff`, `time_updated`) VALUES
(1, 1, 1, 1000, 2000, 1000, '2017-07-21 09:31:38'),
(2, 2, 1, 2000, 5000, 3000, '2017-07-21 09:52:04');

-- --------------------------------------------------------

--
-- Table structure for table `game`
--

CREATE TABLE `game` (
  `id` int(11) NOT NULL,
  `game` varchar(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `game`
--

INSERT INTO `game` (`id`, `game`) VALUES
(1, 'Slots'),
(2, 'Roulette'),
(3, 'BlackJack'),
(4, 'Baccarat'),
(5, 'Casino War');

-- --------------------------------------------------------

--
-- Table structure for table `history`
--

CREATE TABLE `history` (
  `id` int(11) NOT NULL,
  `uid` int(11) NOT NULL,
  `gid` int(11) NOT NULL,
  `bet` bigint(20) NOT NULL,
  `win` bigint(20) NOT NULL,
  `diff` bigint(20) NOT NULL,
  `time_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `history`
--

INSERT INTO `history` (`id`, `uid`, `gid`, `bet`, `win`, `diff`, `time_created`) VALUES
(1, 1, 1, 500, 1000, -500, '2017-07-21 08:51:16'),
(2, 1, 1, 400, 200, -200, '2017-07-21 08:51:16'),
(3, 1, 1, 1000, 2000, 1000, '2017-07-22 04:12:12'),
(4, 1, 1, 2000, 4000, 2000, '2017-07-22 04:12:12'),
(5, 1, 2, 4000, 6000, -2000, '2017-07-22 04:12:39'),
(6, 1, 2, 3000, 7000, 3000, '2017-07-22 04:12:39'),
(7, 1, 1, 1111, 2111, 1000, '2017-07-23 05:27:25'),
(8, 1, 1, 1111, 2111, 1000, '2017-07-23 05:28:25');

-- --------------------------------------------------------

--
-- Table structure for table `sysadmin`
--

CREATE TABLE `sysadmin` (
  `id` int(11) NOT NULL,
  `name` varchar(20) NOT NULL,
  `password` varchar(20) NOT NULL,
  `privileges` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sysadmin`
--

INSERT INTO `sysadmin` (`id`, `name`, `password`, `privileges`) VALUES
(4, 'leonipodoan', 'abcde', '[\"0\",\"1\",\"4\"]');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` bigint(20) NOT NULL,
  `fid` varchar(20) NOT NULL,
  `username` varchar(20) NOT NULL,
  `password` varchar(20) NOT NULL,
  `email_address` text NOT NULL,
  `dev_android` text NOT NULL,
  `dev_android_location` varchar(10) NOT NULL,
  `dev_ios` text NOT NULL,
  `dev_ios_location` varchar(10) NOT NULL,
  `my_coins` bigint(20) NOT NULL,
  `money_spent` float NOT NULL,
  `message` text NOT NULL,
  `time_updated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `time_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`id`, `fid`, `username`, `password`, `email_address`, `dev_android`, `dev_android_location`, `dev_ios`, `dev_ios_location`, `my_coins`, `money_spent`, `message`, `time_updated`, `time_created`) VALUES
(1, '12345', 'hammyliem', '', 'hammyliem@gmail.com', '12312', 'open', '1231231', 'idk', 0, 8.19, '', '2017-07-21 20:39:13', '0000-00-00 00:00:00'),
(2, '12345', 'leonipodoan', '', 'hammyliem@gmail.com', '12312', 'open', '1231231', 'idk', 7000, 8.19, 'wht the fuck!', '2017-07-21 04:50:52', '0000-00-00 00:00:00'),
(3, '1234523', 'daniel', '', 'hammyliem@gmail.com', '12312', 'open', '1231231', 'idk', 10000, 8.19, '', '2017-07-23 05:08:39', '0000-00-00 00:00:00'),
(4, '912939123', 'rickysaputra', 'abce', 'adfasdf', 'sda', 'sadf', 'sadf', '', 0, 0, '', '2017-07-23 05:00:36', '2017-07-23 05:00:36');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `all_time_leaderboard`
--
ALTER TABLE `all_time_leaderboard`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- Indexes for table `game`
--
ALTER TABLE `game`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `history`
--
ALTER TABLE `history`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `sysadmin`
--
ALTER TABLE `sysadmin`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `all_time_leaderboard`
--
ALTER TABLE `all_time_leaderboard`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT for table `game`
--
ALTER TABLE `game`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT for table `history`
--
ALTER TABLE `history`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
--
-- AUTO_INCREMENT for table `sysadmin`
--
ALTER TABLE `sysadmin`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `id` bigint(20) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
