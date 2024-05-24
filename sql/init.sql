SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

-- -----------------------------------------------------
-- Table `hackathon`.`user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `hackathon`.`user` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `login` VARCHAR(255) NOT NULL ,
  `password` VARCHAR(255) NOT NULL,
  `registered_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL ,
  `last_visited_at` TIMESTAMP NOT NULL) 
ENGINE = InnoDB;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;