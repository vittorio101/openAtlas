﻿------------------------------------------------------------------------------
--                                                                          --
--  This file is part of openATLAS.                                         --
--                                                                          --
--    openATLAS is free software: you can redistribute it and/or modify     --
--    it under the terms of the GNU General Public License as published by  --
--    the Free Software Foundation, either version 3 of the License, or     --
--    any later version.                                                    --
--                                                                          --
--    openATLAS is distributed in the hope that it will be useful,          --
--    but WITHOUT ANY WARRANTY; without even the implied warranty of        --
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         --
--    GNU General Public License for more details.                          --
--                                                                          --
--    You should have received a copy of the GNU General Public License     --
--    along with openATLAS. If not, see <http://www.gnu.org/licenses/>.     --
--                                                                          --
--    Database Design by Stefan Eichert 2013, stefan.eichert@univie.ac.at   --
--    Frontend developed by Viktor Jansa 2013, viktor.jansa@gmx.net         --
--                                                                          --
------------------------------------------------------------------------------



--This script sets up the database backend for openATLAS and inserts the necessary data
--requirements: Postgres Database with functioning PostGIS extension. Tested with Postgres Version 9.2.4.


--schema
DROP SCHEMA IF EXISTS openatlas CASCADE;
CREATE SCHEMA openatlas;


-- tbl_classes

DROP TABLE IF EXISTS openatlas.tbl_classes CASCADE;
CREATE TABLE openatlas.tbl_classes
-- The table tbl_classes contains the list of classes used in Open Atlas. 
-- They are derived from the  CIDOC-CRM (http://www.cidoc-crm.org/). 
-- While the name may be translated or modified the meaning must remain the same
-- The CIDOC-Number is used to clearly identify the class

(
  tbl_classes_uid serial NOT NULL, -- auto incrementing ongoing number
  cidoc_class_nr character varying(10), -- CIDOC-Nr. 
  cidoc_class_original_name character varying(100) NOT NULL, -- original name of the class
  cidoc_class_name_translated character varying(100) NOT NULL, -- translated name of the class 
  class_description text, -- to whom it may concern
  class_parent_cidoc_nr character varying (50), --parent class
  
  
  CONSTRAINT tbl_classes_pkey PRIMARY KEY (tbl_classes_uid),
  CONSTRAINT tbl_classes_cidoc_class_name_translated_key UNIQUE (cidoc_class_name_translated),
  CONSTRAINT tbl_classes_cidoc_class_original_name_key UNIQUE (cidoc_class_original_name)
);


--sample data classes required (if new class is added, add it at the end for the uid of the other classes to remain the same)
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E001', 'CRM entity', 'CRM Entität', NULL, 'E000');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E002', 'temporal entity', 'Geschehendes', NULL, 'E001');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E004', 'period', 'Kulturphase', NULL, 'E002');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E053', 'place', 'Ort', NULL, 'E001');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E077', 'persistent item', 'Seiendes', NULL, 'E001');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E070', 'thing', 'Sache', NULL, 'E077');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E071', 'man-made thing', 'Künstliches', NULL, 'E070');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E028', 'conceptual object', 'Begrifflicher Gegenstand', NULL, 'E071');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E089', 'propositional object', 'Aussagenobjekt', NULL, 'E028');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E073', 'information object', 'Informationsgegenstand', NULL, 'E089');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E031', 'document', 'Dokument', NULL, 'E071');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E018', 'physical thing', 'Materielles', NULL, 'E070');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E055', 'type', 'Typus', NULL, 'E071');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E052', 'timespan', 'Zeitspanne', NULL, 'E001');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E019', 'physical object', 'Materieller Gegenstand', NULL, 'E018');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E039', 'actor', 'Akteur', NULL, 'E077');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E021', 'person', 'Person', NULL, 'E039');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E058', 'measurement unit', 'Masseinheit', NULL, 'E055');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E057', 'material', 'Material', NULL, 'E055');
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E000', 'class root', 'Klasse Ursprung', NULL, NULL);
INSERT INTO openatlas.tbl_classes VALUES (DEFAULT, 'E030', 'right', 'Recht', NULL, 'E089');

--tbl_properties
DROP TABLE IF EXISTS openatlas.tbl_properties CASCADE;
CREATE TABLE openatlas.tbl_properties
-- The table properties contains the a list of properties used in Open Atlas. 
-- They are derived from the CIDOC-CRM (http://www.cidoc-crm.org/). 
-- While the name may be translated or modified the meaning must remain the same
-- The CIDOC-Number in combination with the property_from_original value is used to clearly identify the property and its direction
-- one entry has a source class and a target class

(
  tbl_properties_uid serial NOT NULL, -- auto incrementig ongoing number
  property_cidoc_number character varying(10), -- plain CIDOC-Nr.
  property_cidoc_number_direction character varying(10), --cidoc number + "a" or "b" is used for an exact definition for the source to target relation
  property_from_original character varying(250) NOT NULL, -- name of the property (source to target)
  property_to_original character varying(250), -- name of property (target to source)
  property_to_number character varying(10), --cidoc number + "a" or "b" is used for an exact definition for the source to target relation
  property_from_translated character varying(250) NOT NULL, -- name of the property, translated (source to target)
  property_to_translated character varying(250), -- name of property, translated (target to source)
  property_description text, -- to whom it may concern
  property_domain character varying(10), --top parent class that may have the property
  property_range character varying(10), --top parent class that this property can lead to

  
  
  CONSTRAINT tbl_properties_pkey PRIMARY KEY (tbl_properties_uid) -- property_cidoc_number_direction uid is used as a foreign key in other tables
);


INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P002', 'P002a', 'has type', 'is type of', 'P002b', 'hat den Typus', 'ist Typus von', NULL, 'E001', 'E055');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P002', 'P002b', 'is type of', 'has type', 'P002a', 'ist Typus von', 'hat den Typus', NULL, 'E055', 'E001');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P070', 'P070a', 'documents', 'is documented in',  'P070b', 'beschreibt', 'wird beschrieben in', NULL, 'E031', 'E001');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P070', 'P070b', 'is documented in', 'documents', 'P070a', 'wird beschrieben in', 'beschreibt', NULL, 'E001', 'E031');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P089', 'P089a', 'falls within (spatial)', 'contains (spatial)', 'P089b', 'liegt räumlich in', 'enthält räumlich', NULL, 'E053', 'E053');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P089', 'P089b', 'contains (spatial)', 'falls within (spatial)', 'P089a', 'enthält räumlich', 'liegt räumlich in', NULL, 'E053', 'E053');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P127', 'P127a', 'has broader term', 'has narrower term', 'P127b', 'fällt in die Überkategorie', 'enthält die Unterkategorie', NULL, 'E055', 'E055');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P127', 'P127b', 'has narrower term', 'has broader term', 'P127a', 'enthält die Unterkategorie', 'fällt in die Überkategorie', NULL, 'E055', 'E055');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P010', 'P010a', 'falls within (chronological)', 'contains (chronological)', 'P010b', 'fällt zeitlich in', 'enthält zeitlich', NULL, 'E004', 'E004');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P010', 'P010b', 'contains (chronological)', 'falls within (chronological)', 'P010a', 'enthält zeitlich', 'fällt zeitlich in', NULL, 'E004', 'E004');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P046', 'P046a', 'is composed of', 'forms part of', 'P046b', 'ist zusammengesetzt aus', 'bildet Teil von', NULL, 'E018', 'E018');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P046', 'P046b', 'forms part of', 'is composed of', 'P046a', 'bildet Teil von', 'ist zusammengesetzt aus', NULL, 'E018', 'E018');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P086', 'P086a', 'falls within (chronological)', 'contains (chronological)', 'P086b', 'fällt zeitlich in', 'enthält zeitlich', NULL, 'E052', 'E052');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P086', 'P086b', 'contains (chronological)', 'falls within (chronological)', 'P086a', 'enthält zeitlich', 'fällt zeitlich in', NULL, 'E052', 'E052');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P053', 'P053a', 'has former or current location', 'is former or current location of', 'P053b', 'hat Standort', 'ist Standort von', NULL, 'E018', 'E053');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P053', 'P053b', 'is former or current location of', 'has former or current location', 'P053a', 'ist Standort von', 'hat Standort', NULL, 'E053', 'E018');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P104', 'P104a', 'is subject to', 'applies to', 'P104b', 'Gegenstand von', 'findet Anwendung auf', NULL, 'E072', 'E030');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P104', 'P104b', 'applies to', 'is subject to', 'P104a', 'findet Anwendung auf', 'Gegenstand von', NULL, 'E030', 'E072');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P105', 'P105a', 'right held by', 'has right on', 'P105b', 'Rechte gehören', 'hat Rechte an', NULL, 'E072', 'E039');
INSERT INTO openatlas.tbl_properties VALUES (DEFAULT, 'P105', 'P105b', 'has right on', 'right held by', 'P105a', 'hat Rechte an', 'Rechte gehören', NULL, 'E039', 'E072');


-- tbl_entities

DROP TABLE IF EXISTS openatlas.tbl_entities CASCADE;
CREATE TABLE openatlas.tbl_entities
-- The table contains all entities recorded in openatlas
-- The user_id should be defined as a default value within the gui
-- Triggers in the database should do the following: 
--  composition of the entity_id
--  timestamps for creation and update + user that did the last edit
--  calculate geometry field from text (given: x-y coordinates and SRID) – on insert and update
--  calculate x-y coordinates from geometry (given geometry and SRID) – on insert and update
(
  --functional
  uid serial NOT NULL, -- ongoig number, auto increment
  user_id character varying(10) NOT NULL DEFAULT 'sys'::character varying, -- user id (to be inserted by the gui)
  entity_id character varying(250) NOT NULL, -- unique entity  ID automatically composed by adding user-id and an ongoing number 
  timestamp_creation timestamp(0) DEFAULT NOW(), -- timestamp of the creation automatically composed 
  timestamp_edit timestamp(0), -- timestamp of last edit automatically composed 
  user_edit character varying(10), -- user responsible for the last edit (to be inserted by the gui)
  
  --identification
  classes_uid integer NOT NULL, -- class_nr of the entity (original cidoc nr)
  entity_name_uri character varying(250), -- name resp. unique appelation of entity
  entity_type integer, -- main type of entity
  entity_description text, -- to whom it may concern
  entity_remarks text, -- to whom it may concern

  --temporal (and used to determine the order of several entities that should not be ordered alphabetically)
  start_time_abs integer, -- chronological beginnig of dating (year) or in case of class "period" if no absolute chronological frame is given: a value for the order of it
  end_time_abs integer, --  chronological end of dating (year)
  start_time_text character varying(50), -- additional text to handle fuzzy temporal information
  end_time_text character varying(50), -- additional text to handle fuzzy temporal information

  --dimensions
  dim_width double precision, -- width of entity if exists
  dim_length double precision, -- length of entity if exists
  dim_height double precision, -- height of entity if exists
  dim_thickness double precision, -- thickness of entity if exists
  dim_diameter double precision, -- diameter of entity if exists
  dim_units character varying(20), -- units of measurement like meters, centimeters...
  dim_weight double precision, -- weight of the entity if exists
  dim_units_weight character varying (20), -- units of the weight like kilogram, ton, gram
  dim_degrees integer, -- degrees (360° for full circle, clockwise, north = 0, east = 90...)

    --spatial
  x_lon_easting double precision, -- X-coordinate
  y_lat_northing double precision, -- Y-coordinate
  srid_epsg integer, -- EPSG Code /SRID of the coord. System

  

  CONSTRAINT tbl_entities_pkey PRIMARY KEY (uid),
  CONSTRAINT tbl_entities_entity_class_nr_fkey FOREIGN KEY (classes_uid),
        REFERENCES openatlas.tbl_classes (tbl_classes_uid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE NO ACTION,
  CONSTRAINT entity_id_unique UNIQUE (entity_id)
);

  -- + geom(Point), -- geometry field created by the postgis-extension with SELECT AddGeometryColumn (..)
SELECT AddGeometryColumn ( 'openatlas', 'tbl_entities', 'geom', -1, 'POINT', 2 );


--functions on table entities



--geometry from xy value and vice versa on creation
CREATE OR REPLACE FUNCTION openatlas.geometry_creation()
  RETURNS trigger AS
  $BODY$
  BEGIN
   IF (NEW.geom IS NULL) THEN
    NEW.geom = ST_MakePoint(NEW.x_lon_easting, NEW.y_lat_northing);
   END IF;
   
   IF (NEW.x_lon_easting IS NULL) THEN
    NEW.x_lon_easting = ST_X(NEW.geom);
    NEW.y_lat_northing = ST_Y(NEW.geom);
   END IF;

   IF (NEW.y_lat_northing IS NULL) THEN
    NEW.x_lon_easting = ST_X(NEW.geom);
    NEW.y_lat_northing = ST_Y(NEW.geom);
   END IF;

    NEW.x_lon_easting = ST_X(NEW.geom);
    NEW.y_lat_northing = ST_Y(NEW.geom); 
    
    
   RETURN NEW;
  END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;

--geometry from xy value and vice versa on update
CREATE OR REPLACE FUNCTION openatlas.geometry_update()
  RETURNS trigger AS
  $BODY$
  BEGIN
   IF (NEW.geom IS NULL) THEN
    NEW.geom = ST_MakePoint(NEW.x_lon_easting, NEW.y_lat_northing);
   END IF;
   
   IF (NEW.x_lon_easting IS NULL) THEN
    NEW.x_lon_easting = ST_X(NEW.geom);
    NEW.y_lat_northing = ST_Y(NEW.geom);
   END IF;

   IF (NEW.y_lat_northing IS NULL) THEN
    NEW.x_lon_easting = ST_X(NEW.geom);
    NEW.y_lat_northing = ST_Y(NEW.geom);
   END IF;


   IF ((NEW.x_lon_easting IS DISTINCT FROM OLD.x_lon_easting) OR (NEW.y_lat_northing IS DISTINCT FROM OLD.y_lat_northing)) THEN
    NEW.geom = ST_MakePoint(NEW.x_lon_easting, NEW.y_lat_northing);  
   END IF;  

   NEW.x_lon_easting = ST_X(NEW.geom);
   NEW.y_lat_northing = ST_Y(NEW.geom);

    
   RETURN NEW;
  END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
  
--creation of timestamps, id
CREATE OR REPLACE FUNCTION openatlas.autocreate()
  RETURNS trigger AS 
  $BODY$
  BEGIN
    IF NEW.entity_id IS NULL THEN
    NEW.entity_id = NEW.user_id || '_' || NEW.uid;
    NEW.timestamp_creation = now();
    END IF;
    
    NEW.timestamp_edit = now();

         
    RETURN NEW;
  END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;




--trigger

DROP TRIGGER IF EXISTS geometry_creation ON openatlas.tbl_entities;
CREATE TRIGGER geometry_creation
  BEFORE INSERT
  ON openatlas.tbl_entities
  FOR EACH ROW
  EXECUTE PROCEDURE openatlas.geometry_creation();


DROP TRIGGER IF EXISTS geometry_update ON openatlas.tbl_entities;
CREATE TRIGGER geometry_update
  BEFORE UPDATE
  ON  openatlas.tbl_entities
  FOR EACH ROW
  EXECUTE PROCEDURE openatlas.geometry_update();


DROP TRIGGER IF EXISTS autocreate ON openatlas.tbl_entities;
CREATE TRIGGER autocreate
  BEFORE INSERT OR UPDATE
  ON  openatlas.tbl_entities
  FOR EACH ROW
  EXECUTE PROCEDURE openatlas.autocreate();


--required data Types
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_003', 13, 'Feature');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_269', 13, 'Finds');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_277', 13, 'Site');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_279', 13, 'Stratigraphical Unit');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_282', 13, 'Evidence');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_283', 13, 'History');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_286', 13, 'Archaeology');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_293', 13, 'Reference');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_294', 13, 'Image');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_295', 13, 'Text');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_296', 13, 'Scientific Literature');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_297', 13, 'Primary Source');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_298', 13, 'Cultural Period');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_307', 13, 'Chronological Period');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_322', 13, 'Types');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_323', 13, 'Administrative Unit');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_324', 13, 'Continent');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_321', 13, 'open_atlas_hierarchy');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_331', 13, 'Measurement Units');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_332', 13, 'Distance');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_333', 18, 'Kilometer');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_334', 18, 'Meter');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_335', 18, 'Centimeter');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_336', 18, 'Millimeter');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_337', 13, 'Weight');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_338', 18, 'Ton');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_339', 18, 'Kilogram');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Typ_340', 18, 'Gram');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Sex_001', 13, 'Sex');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Sex_002', 13, 'Male');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Sex_003', 13, 'Female');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Age_001', 13, 'Age');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Age_002', 13, 'Adult');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Age_003', 13, 'Child');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri) VALUES ('Mat_001', 13, 'Material');


--sample data cultural periods required
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs) VALUES ('Per_001', 3, 'Cultural Periods', 13, '0');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs) VALUES ('Per_002', 3, 'Stone Age', 13, '-2000000');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs) VALUES ('Per_003', 3, 'Bronze Age', 13, '-3000');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs) VALUES ('Per_004', 3, 'Iron Age', 13, '-800');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs) VALUES ('Per_005', 3, 'Roman', 13, '1');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs) VALUES ('Per_006', 3, 'Medieval', 13, '500');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs) VALUES ('Per_007', 3, 'Modern Age', 13, '1500');

--sample data chronological periods required
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs, end_time_abs) VALUES ('chr_001', 14, 'Chronological Periods', 14, '-99999999', '2000');
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type, start_time_abs, end_time_abs) VALUES ('chr_002', 14, 'Human history', 14, '-2000000', '2000');

--sample data places required
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type) VALUES ('pla_001', 4, 'Places', NULL);
INSERT INTO openatlas.tbl_entities (entity_id, classes_uid, entity_name_uri, entity_type) VALUES ('pla_002', 4, 'Europe', 17);






-- tbl_links
DROP TABLE IF EXISTS openatlas.tbl_links CASCADE;
CREATE TABLE openatlas.tbl_links
-- The table links contains the links between one entity and another via a certain property
(
  links_uid serial NOT NULL, -- autoincrement, ongoing Nr. 
  links_entity_uid_from integer NOT NULL, -- source entity uid
  links_cidoc_number_direction integer NOT NULL, -- cidoc code of the property + direction as defined in tbl_properties
  links_entity_uid_to integer NOT NULL, -- target entity uid
  links_annotation text, -- remarks, description etc. E.g. for declaring a page number in case of entity of class document „refers to“ entity of class thing ecc. 
  links_creator character varying(50) NOT NULL DEFAULT 'sys'::character varying (50), -- username of the link's creator
  links_timestamp_start timestamp(0), -- date or time when the property begins to be linked to the entity
  links_timestamp_end timestamp(0), -- date or time when the property ends to be linked to the entity
  links_timestamp_creation timestamp(0) DEFAULT NOW(), -- timestamp of the creation automatically composed 
  links_timespan integer, -- duration of the link i.e. the timespan in which the property links the two entities (uid of a certain E52 entity)
  CONSTRAINT tbl_links_pkey PRIMARY KEY (links_uid),
  CONSTRAINT tbl_links_links_cidoc_number_direction_fkey FOREIGN KEY (links_cidoc_number_direction)
      REFERENCES openatlas.tbl_properties (tbl_properties_uid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT tbl_links_links_entity_id_from_fkey FOREIGN KEY (links_entity_uid_from)
      REFERENCES openatlas.tbl_entities (uid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE CASCADE,
  CONSTRAINT tbl_links_links_entity_id_to_fkey FOREIGN KEY (links_entity_uid_to)
      REFERENCES openatlas.tbl_entities (uid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE CASCADE
);


--sample data links
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'pla_001'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_321'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'chr_001'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_321'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_001'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_321'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_003'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_269'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_277'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_279'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_282'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_283'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_282'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_286'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_282'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_293'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_294'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_293'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_295'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_293'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_296'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_295'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_297'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_295'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_298'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_307'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_321'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_323'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_324'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_323'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_331'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_332'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_331'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_333'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_332'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_334'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_332'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_335'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_332'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_336'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_332'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_337'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_331'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_338'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_337'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_339'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_337'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_340'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_337'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Mat_001'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Sex_001'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Sex_002'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Sex_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Sex_003'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Sex_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Age_001'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Typ_322'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Age_002'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Age_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Age_003'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P127a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Age_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'pla_002'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P089a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'pla_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'chr_002'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P086a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'chr_001'));  
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_002'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P010a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_003'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P010a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_004'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P010a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_005'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P010a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_006'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P010a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_001'));
INSERT INTO openatlas.tbl_links (links_entity_uid_from, links_cidoc_number_direction, links_entity_uid_to) VALUES ((SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_007'), (SELECT tbl_properties_uid serial FROM openatlas.tbl_properties WHERE property_cidoc_number_direction = 'P010a'), (SELECT uid FROM openatlas.tbl_entities WHERE entity_id = 'Per_001'));

--gis_tables
--they store geometry data connected to certain entities. Each entity may have many GIS data connected (1:n)

-- tbl_gis_linestring
DROP TABLE IF EXISTS openatlas.tbl_gis_linestring CASCADE;
CREATE TABLE openatlas.tbl_gis_linestring
-- table that contains linestring geometries
(
  linestring_uid serial NOT NULL, -- autoincrement, ongoing Nr. 
  parent_entity_id character varying(100), -- ID from parent entity
  parent_uid integer, -- uid of parent
  object_name character varying(100), -- to whom it may concern
  object_description text, -- to whom it may concern
  srid_epsg integer, -- EPSG Code /SRID of the coord. System
  CONSTRAINT tbl_gis_linestring_pkey PRIMARY KEY (linestring_uid),
  CONSTRAINT tbl_gis_linestring_parent_entity_id_fkey FOREIGN KEY (parent_uid)
      REFERENCES openatlas.tbl_entities (uid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE CASCADE
  );

SELECT AddGeometryColumn ( 'openatlas', 'tbl_gis_linestring', 'geom', -1, 'LINESTRING', 2 );

-- tbl_gis_polygons
DROP TABLE IF EXISTS openatlas.tbl_gis_polygons CASCADE;
CREATE TABLE openatlas.tbl_gis_polygons
-- table that contains polygon geometries
(
  polygons_uid serial NOT NULL, -- autoincrement, ongoing Nr. 
  parent_entity_id character varying(100), -- ID from parent entity
  parent_uid integer, -- uid of parent
  object_name character varying(100), -- to whom it may concern
  object_description text, -- to whom it may concern
  srid_epsg integer, -- EPSG Code /SRID of the coord. System
  CONSTRAINT tbl_gis_polygons_pkey PRIMARY KEY (polygons_uid),
  CONSTRAINT tbl_gis_polygons_parent_entity_id_fkey FOREIGN KEY (parent_uid)
      REFERENCES openatlas.tbl_entities (uid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE CASCADE
  );

SELECT AddGeometryColumn ( 'openatlas', 'tbl_gis_polygons', 'geom', -1, 'POLYGON', 2 );

-- tbl_gis_points
DROP TABLE IF EXISTS openatlas.tbl_gis_points CASCADE;
CREATE TABLE openatlas.tbl_gis_points
-- table that contains polygon geometries
(
  points_uid serial NOT NULL, -- autoincrement, ongoing Nr. 
  parent_entity_id character varying(100), -- ID from parent entity
  parent_uid integer, -- uid of parent
  object_name character varying(100), -- to whom it may concern
  object_description text, -- to whom it may concern
  srid_epsg integer, -- EPSG Code /SRID of the coord. System
  CONSTRAINT tbl_gis_points_pkey PRIMARY KEY (points_uid),
  CONSTRAINT tbl_gis_points_parent_entity_id_fkey FOREIGN KEY (parent_uid)
      REFERENCES openatlas.tbl_entities (uid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE CASCADE
  );

SELECT AddGeometryColumn ( 'openatlas', 'tbl_gis_points', 'geom', -1, 'POINT', 2 );



--Views:
  ---One parent_child view that shows the parent of each entity resp. the two nodes of a connection from tbl_links
  ---One tree view that recursively queries the path of the entity

--view parent_child_all (needed for treeview)
 --show all connections between entities via tbl_links

CREATE OR REPLACE VIEW openatlas.parent_child_all AS 
 SELECT 
    tbl_entities.uid AS child_uid,
    tbl_entities.entity_name_uri AS child_name,
    tbl_links.links_cidoc_number_direction AS link,
    tbl_entities_1.uid AS parent_uid, 
    tbl_entities_1.entity_name_uri AS parent_name 
    
   FROM openatlas.tbl_entities
   JOIN openatlas.tbl_links ON tbl_entities.uid = tbl_links.links_entity_uid_from
   JOIN openatlas.tbl_entities tbl_entities_1 ON tbl_links.links_entity_uid_to = tbl_entities_1.uid;
   
--all_tree (needed for treeview)
 --recursiv view with path
CREATE OR REPLACE VIEW openatlas.all_tree AS
   WITH RECURSIVE path(name, path, parent, uid, parent_uid) AS (
                 SELECT openatlas.parent_child_all.child_name, ''::text || openatlas.parent_child_all.child_name::text AS path, NULL::text AS text, openatlas.parent_child_all.child_uid, openatlas.parent_child_all.parent_uid
                   FROM openatlas.parent_child_all
                  WHERE openatlas.parent_child_all.parent_name::text ~~ 'open_atlas_hierarchy'::text -- replace value with parent of top-categorie you want to have displayed
        UNION ALL 
                 SELECT openatlas.parent_child_all.child_name, (parentpath.path || 
                        CASE parentpath.path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.parent_child_all.child_name::text, parentpath.path, openatlas.parent_child_all.child_uid, openatlas.parent_child_all.parent_uid
                   FROM openatlas.parent_child_all, path parentpath
                  WHERE openatlas.parent_child_all.parent_uid::text = parentpath.uid::text
        )
 SELECT path.name, path.path, path.parent, path.uid, path.parent_uid
   FROM path
  ORDER BY path.path;



--view types parent_child (needed for recursive view "types_all_tree")

CREATE OR REPLACE VIEW openatlas.types_parent_child AS 
 SELECT 
    tbl_entities_1.uid AS parent_id,
    tbl_entities_1.entity_name_uri AS parent_name, 
    tbl_entities.uid AS child_id, 
    tbl_entities.entity_name_uri AS child_name
   FROM openatlas.tbl_entities
   JOIN openatlas.tbl_links ON tbl_entities.uid = tbl_links.links_entity_uid_from
   JOIN openatlas.tbl_entities tbl_entities_1 ON tbl_links.links_entity_uid_to = tbl_entities_1.uid
  WHERE tbl_links.links_cidoc_number_direction = 7 AND (tbl_entities.classes_uid = 13 OR tbl_entities.classes_uid = 18 OR tbl_entities.classes_uid = 19);



--types_all_tree
CREATE OR REPLACE VIEW openatlas.types_all_tree AS
   WITH RECURSIVE path(id, path, parent, name, parent_id, name_path) AS (
                 SELECT openatlas.types_parent_child.child_id, ''::text || openatlas.types_parent_child.child_id::text AS path, NULL::text AS text, openatlas.types_parent_child.child_name, openatlas.types_parent_child.parent_id, ''::text || openatlas.types_parent_child.child_name::text AS name_path
                   FROM openatlas.types_parent_child
                  WHERE openatlas.types_parent_child.parent_name::text ~~ 'open_atlas_hierarchy'::text -- replace value with parent of top-categorie you want to have displayed
        UNION ALL 
                 SELECT openatlas.types_parent_child.child_id, (parentpath.path || 
                        CASE parentpath.path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.types_parent_child.child_id::text, parentpath.path, openatlas.types_parent_child.child_name, openatlas.types_parent_child.parent_id, 

		    (parentpath.name_path || 
                        CASE parentpath.name_path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.types_parent_child.child_name::text

                   FROM openatlas.types_parent_child, path parentpath
                  WHERE openatlas.types_parent_child.parent_id::text = parentpath.id::text
        )
        
 SELECT path.name, path.id, path.path, path.parent_id, name_path
   FROM path
  ORDER BY path.path;



-- view_cultural_period_parent_child (needed for recursive view "cultural_period_all_tree")
CREATE OR REPLACE VIEW openatlas.cultural_period_parent_child AS 
 SELECT tbl_entities_1.uid AS parent_id, 
    tbl_entities_1.entity_name_uri AS parent_name, 
    tbl_entities.uid AS child_id, 
    tbl_entities.entity_name_uri AS child_name,
    tbl_entities.start_time_abs AS order_sequence
   FROM openatlas.tbl_entities
   JOIN openatlas.tbl_links ON tbl_entities.uid = tbl_links.links_entity_uid_from
   JOIN openatlas.tbl_entities tbl_entities_1 ON tbl_links.links_entity_uid_to = tbl_entities_1.uid
  WHERE tbl_links.links_cidoc_number_direction = 9 AND tbl_entities.classes_uid = 3;

  
--cultural_period_all_tree
CREATE OR REPLACE VIEW openatlas.cultural_period_all_tree AS
WITH RECURSIVE path(id, path, parent, name, parent_id, name_path, order_sequence) AS (
                 SELECT openatlas.cultural_period_parent_child.child_id, ''::text || openatlas.cultural_period_parent_child.child_id::text AS path, NULL::text AS text, openatlas.cultural_period_parent_child.child_name, openatlas.cultural_period_parent_child.parent_id, ''::text || openatlas.cultural_period_parent_child.child_name::text AS name_path, openatlas.cultural_period_parent_child.order_sequence
                   FROM openatlas.cultural_period_parent_child
                  WHERE openatlas.cultural_period_parent_child.parent_name::text ~~ 'Cultural Periods'::text -- replace value with parent of top-categorie you want to have displayed
        UNION ALL 
                 SELECT openatlas.cultural_period_parent_child.child_id, (parentpath.path || 
                        CASE parentpath.path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.cultural_period_parent_child.child_id::text, parentpath.path, openatlas.cultural_period_parent_child.child_name, openatlas.cultural_period_parent_child.parent_id, 

		    (parentpath.name_path || 
                        CASE parentpath.name_path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.cultural_period_parent_child.child_name::text, openatlas.cultural_period_parent_child.order_sequence

                   FROM openatlas.cultural_period_parent_child, path parentpath
                  WHERE openatlas.cultural_period_parent_child.parent_id::text = parentpath.id::text
        )
        
 SELECT path.name, path.id, path.path, path.parent_id, name_path, path.order_sequence
 FROM path
 ORDER BY path.order_sequence;
  
-- view chronological_period_parent_child (needed for recursive view "cultural_period_all_tree")
CREATE OR REPLACE VIEW openatlas.chronological_period_parent_child AS 
 SELECT tbl_entities_1.uid AS parent_id, 
    tbl_entities_1.entity_name_uri AS parent_name, 
    tbl_entities.uid AS child_id, 
    tbl_entities.entity_name_uri AS child_name,
    tbl_entities.start_time_abs AS start_time,
    tbl_entities.end_time_abs AS end_time
   FROM openatlas.tbl_entities
   JOIN openatlas.tbl_links ON tbl_entities.uid = tbl_links.links_entity_uid_from
   JOIN openatlas.tbl_entities tbl_entities_1 ON tbl_links.links_entity_uid_to = tbl_entities_1.uid
  WHERE tbl_links.links_cidoc_number_direction = 13 AND tbl_entities.entity_type = 14
  ORDER BY start_time ASC, end_time DESC;

--chronological_period_all_tree
CREATE OR REPLACE VIEW openatlas.chronological_period_all_tree AS
 WITH RECURSIVE path(id, path, parent, name, parent_id, name_path, start_time, end_time) AS (
                 SELECT openatlas.chronological_period_parent_child.child_id, ''::text || openatlas.chronological_period_parent_child.child_id::text AS path, NULL::text AS text, openatlas.chronological_period_parent_child.child_name, openatlas.chronological_period_parent_child.parent_id, ''::text || openatlas.chronological_period_parent_child.child_name::text AS name_path, openatlas.chronological_period_parent_child.start_time, openatlas.chronological_period_parent_child.end_time
                   FROM openatlas.chronological_period_parent_child
                  WHERE openatlas.chronological_period_parent_child.parent_name::text ~~ 'Chronological Periods'::text -- replace value with parent of top-categorie you want to have displayed
        UNION ALL 
                 SELECT openatlas.chronological_period_parent_child.child_id, (parentpath.path || 
                        CASE parentpath.path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.chronological_period_parent_child.child_id::text, parentpath.path, openatlas.chronological_period_parent_child.child_name, openatlas.chronological_period_parent_child.parent_id, 

		    (parentpath.name_path || 
                        CASE parentpath.name_path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.chronological_period_parent_child.child_name::text, openatlas.chronological_period_parent_child.start_time, openatlas.chronological_period_parent_child.end_time

                   FROM openatlas.chronological_period_parent_child, path parentpath
                  WHERE openatlas.chronological_period_parent_child.parent_id::text = parentpath.id::text
        )
        
 SELECT path.name, path.id, path.path, path.parent_id, name_path, path.start_time, path.end_time
 FROM path
 ORDER BY path.start_time ASC, path.path ASC;  

-- view place_parent_child (needed for recursive view "place_all_tree")
CREATE OR REPLACE VIEW openatlas.place_parent_child AS
 SELECT tbl_entities_1.uid AS parent_id, 
    tbl_entities_1.entity_name_uri AS parent_name, 
    tbl_entities.uid AS child_id, 
    tbl_entities.entity_name_uri AS child_name,
    tbl_entities.entity_type AS type
   
    
   FROM openatlas.tbl_entities
   JOIN openatlas.tbl_links ON tbl_entities.uid = tbl_links.links_entity_uid_from
   JOIN openatlas.tbl_entities tbl_entities_1 ON tbl_links.links_entity_uid_to = tbl_entities_1.uid
  WHERE tbl_links.links_cidoc_number_direction = 5 AND tbl_entities.classes_uid = 4; --replace with classes nr

--place_all_tree
CREATE OR REPLACE VIEW openatlas.place_all_tree AS  
   WITH RECURSIVE path(id, path, parent, name, parent_id, name_path) AS (
                 SELECT openatlas.place_parent_child.child_id, ''::text || openatlas.place_parent_child.child_id::text AS path, NULL::text AS text, openatlas.place_parent_child.child_name, openatlas.place_parent_child.parent_id, ''::text || openatlas.place_parent_child.child_name::text AS name_path
                   FROM openatlas.place_parent_child
                  WHERE openatlas.place_parent_child.parent_name::text ~~ 'Places'::text -- replace value with parent of top-categorie you want to have displayed
        UNION ALL 
                 SELECT openatlas.place_parent_child.child_id, (parentpath.path || 
                        CASE parentpath.path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.place_parent_child.child_id::text, parentpath.path, openatlas.place_parent_child.child_name, openatlas.place_parent_child.parent_id, 

		    (parentpath.name_path || 
                        CASE parentpath.name_path
                            WHEN ' > '::text THEN ''::text
                            ELSE ' > '::text
                        END) || openatlas.place_parent_child.child_name::text

                   FROM openatlas.place_parent_child, path parentpath
                  WHERE openatlas.place_parent_child.parent_id::text = parentpath.id::text
        )
        
 SELECT path.name, path.id, path.path, path.parent_id, name_path
   FROM path
  ORDER BY path.path;

  

  
-- privileges (activate if necessary

--GRANT ALL ON SCHEMA openatlas TO openatla_jansaviktor; -- replace name and privileges if necessary
--GRANT ALL ON ALL TABLES IN SCHEMA openatlas TO openatla_jansaviktor; -- replace name and privileges if necessary

--GRANT ALL ON SCHEMA openatlas TO openatla_watzingeralex; -- replace name and privileges if necessary
--GRANT ALL ON ALL TABLES IN SCHEMA openatlas TO openatla_watzingeralex; -- replace name and privileges if necessary
   --add rows if necessary

   
   ------------------------------------------------------------------------------
--                                                                          --
--  This file is part of openATLAS.                                         --
--                                                                          --
--    openATLAS is free software: you can redistribute it and/or modify     --
--    it under the terms of the GNU General Public License as published by  --
--    the Free Software Foundation, either version 3 of the License, or     --
--    any later version.                                                    --
--                                                                          --
--    openATLAS is distributed in the hope that it will be useful,          --
--    but WITHOUT ANY WARRANTY; without even the implied warranty of        --
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         --
--    GNU General Public License for more details.                          --
--                                                                          --
--    You should have received a copy of the GNU General Public License     --
--    along with openATLAS. If not, see <http://www.gnu.org/licenses/>.     --
--                                                                          --
--    Database Design by Stefan Eichert 2013, stefan.eichert@univie.ac.at   --
--    Frontend developed by Viktor Jansa 2013, viktor.jansa@gmx.net         --
--                                                                          --
------------------------------------------------------------------------------


--this script sets up tables with types and their paths for sites, features, stratigraphical units and finds
--these tables are used in qgis to be joined with tbl_entities for to display the various entities by their types
--this script should be called from the gui after changing or adding types


-- tbl_gis_sitetypes

DROP TABLE IF EXISTS openatlas.tbl_gis_sitetypes CASCADE;
CREATE TABLE openatlas.tbl_gis_sitetypes
-- The table contains the names and paths of sites' types 
-- It is used for a join in qgis

(
  uid integer NOT NULL, -- same uid as in tbl_entities
  type_name character varying(250), -- name resp. unique appelation of entity
  path character varying(250) -- to whom it may concern
  
);

GRANT ALL ON openatlas.tbl_gis_sitetypes TO public;

INSERT INTO openatlas.tbl_gis_sitetypes (uid, type_name, path) SELECT id, name, name_path FROM openatlas.types_all_tree WHERE name_path LIKE '%Site >%';
UPDATE openatlas.tbl_gis_sitetypes SET path = replace(path, 'Types > Site > ', '');



-- tbl_gis_featuretypes
DROP TABLE IF EXISTS openatlas.tbl_gis_featuretypes CASCADE;
CREATE TABLE openatlas.tbl_gis_featuretypes
-- The table contains the names and paths of features' types 
-- It is used for a join in qgis

(
  uid integer NOT NULL, -- same uid as in tbl_entities
  type_name character varying(250), -- name resp. unique appelation of entity
  path character varying(250) -- to whom it may concern
  
);

GRANT ALL ON openatlas.tbl_gis_featuretypes TO public;

INSERT INTO openatlas.tbl_gis_featuretypes (uid, type_name, path) SELECT id, name, name_path FROM openatlas.types_all_tree WHERE name_path LIKE '%Feature >%';
UPDATE openatlas.tbl_gis_featuretypes SET path = replace(path, 'Types > Feature > ', '');


-- tbl_gis_strat_unitstypes
DROP TABLE IF EXISTS openatlas.tbl_gis_strat_unitstypes CASCADE;
CREATE TABLE openatlas.tbl_gis_strat_unitstypes
-- The table contains the names and paths of strat_units' types 
-- It is used for a join in qgis

(
  uid integer NOT NULL, -- same uid as in tbl_entities
  type_name character varying(250), -- name resp. unique appelation of entity
  path character varying(250) -- to whom it may concern
  
);

GRANT ALL ON openatlas.tbl_gis_strat_unitstypes TO public;

INSERT INTO openatlas.tbl_gis_strat_unitstypes (uid, type_name, path) SELECT id, name, name_path FROM openatlas.types_all_tree WHERE name_path LIKE '%Stratigraphical Unit >%';
UPDATE openatlas.tbl_gis_strat_unitstypes SET path = replace(path, 'Types > Stratigraphical Unit > ', '');

-- tbl_gis_findtypes
DROP TABLE IF EXISTS openatlas.tbl_gis_findtypes CASCADE;
CREATE TABLE openatlas.tbl_gis_findtypes
-- The table contains the names and paths of finds' types 
-- It is used for a join in qgis

(
  uid integer NOT NULL, -- same uid as in tbl_entities
  type_name character varying(250), -- name resp. unique appelation of entity
  path character varying(250) -- to whom it may concern
  
);

GRANT ALL ON openatlas.tbl_gis_findtypes TO public;

INSERT INTO openatlas.tbl_gis_findtypes (uid, type_name, path) SELECT id, name, name_path FROM openatlas.types_all_tree WHERE name_path LIKE '%Finds >%';
UPDATE openatlas.tbl_gis_findtypes SET path = replace(path, 'Types > Finds > ', '');

------------------------------------------------------------------------------
--                                                                          --
--  This file is part of openATLAS.                                         --
--                                                                          --
--    openATLAS is free software: you can redistribute it and/or modify     --
--    it under the terms of the GNU General Public License as published by  --
--    the Free Software Foundation, either version 3 of the License, or     --
--    any later version.                                                    --
--                                                                          --
--    openATLAS is distributed in the hope that it will be useful,          --
--    but WITHOUT ANY WARRANTY; without even the implied warranty of        --
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         --
--    GNU General Public License for more details.                          --
--                                                                          --
--    You should have received a copy of the GNU General Public License     --
--    along with openATLAS. If not, see <http://www.gnu.org/licenses/>.     --
--                                                                          --
--    Database Design by Stefan Eichert 2013, stefan.eichert@univie.ac.at   --
--    Frontend developed by Viktor Jansa 2013, viktor.jansa@gmx.net         --
--                                                                          --
------------------------------------------------------------------------------

--Views/Queries for different Types of real stuff represented in OpenATLAS:
  --Note: if fields are commented by "a" it means that they automatically retrieve their values from triggers in the backend
  --Note: if fields are commented by "v" it means that they should be visible for the user in the GUI
  --Note: if fields are commented by "e" it means that they should be editable for the user in the GUI
  --Note: if fields are uncommented the user should not be able to see/edit them in the gui. 



--site: non-moveable, physical things like settlements, churches, castles, graveyards...
  --  to document: Name, description, type, dating, center coordinates, dimensions
  --  defined by: Cidoc Class Nr = E018/physical thing AND Type = Site (or Subtype of Site)

DROP VIEW IF EXISTS openatlas.sites,
 openatlas.features,
 openatlas.stratigraphical_units,
 openatlas.finds,
 openatlas.texts,
 openatlas.links_evidence, 
 openatlas.links_parents_arch,
 openatlas.links_images,
 openatlas.links_age,
 openatlas.links_sex,
 openatlas.links_bibliography,
 openatlas.links_chronological,
 openatlas.links_cultural,
 openatlas.links_graveconstruction,
 openatlas.links_graveshape,
 openatlas.links_material,
 openatlas.links_places,
 openatlas.links_rights,
 openatlas.links_rightsholder;



CREATE OR REPLACE VIEW openatlas.sites AS  
SELECT 
  tbl_entities.uid, -- a
  tbl_entities.user_id, --default value from the login dialogue
  tbl_entities.entity_id, -- automatically created if NULL
  tbl_entities.timestamp_creation, --a
  tbl_entities.timestamp_edit, --a
  tbl_entities.user_edit, -- the fronted should insert the current users user_id on update
  tbl_entities.classes_uid, -- the frontend should insert the uid of the physical thing class on insert (=12)
  tbl_entities.entity_name_uri, --v e
  tbl_entities.entity_description, -- v e 
  tbl_entities.start_time_abs, -- v e
  tbl_entities.entity_remarks, 
  tbl_entities.end_time_abs, -- v e
  tbl_entities.start_time_text, -- v e
  tbl_entities.end_time_text, -- v e
  tbl_entities.dim_width, -- v e
  tbl_entities.dim_length, -- v e
  tbl_entities.dim_height, -- v e
  tbl_entities.dim_thickness, -- v e
  tbl_entities.dim_diameter, -- v e
  tbl_entities.dim_units, -- v e
  tbl_entities.dim_degrees, -- v e 
  tbl_entities.x_lon_easting, -- v e
  tbl_entities.y_lat_northing, -- v e
  tbl_entities.srid_epsg, -- e
			  -- If NULL then the frontend should automatically insert a Default SRID Number defined in the global settings
                          -- If necessary the user should be able to select another SRID by its name or number in a dialog that saves the chosen srid's number in the srid field of the current dataset)
                          -- In the GUI a Lookup Field should show the SRIDS name
  tbl_entities.geom, -- a
  tbl_entities.entity_type, -- e (In the GUI the user should be able to select the type by its name in a hierarchical organized Select-Dialogue that saves the chosen type's uid in the entity_type field of the current dataset)
  types_all_tree.name_path  -- v
FROM 
  openatlas.types_all_tree, 
  openatlas.tbl_entities
WHERE 
  tbl_entities.entity_type = types_all_tree.id AND
  tbl_entities.classes_uid = 12 AND -- entity has to be a physical thing
  types_all_tree.name_path LIKE '%> Site%'; -- entity's type must be Site or Subtype



--feature: non-moveable, physical things of which a site is composed of like buildings, walls, ditches, graves...
  --  to document: Name, description, type, dating, center coordinates, dimensions
  --  defined by: Cidoc Class Nr = E018/physical thing AND Type = Feature (or Subtype of Feature)

CREATE OR REPLACE VIEW openatlas.features AS  
SELECT 
  tbl_entities.uid, -- a
  tbl_entities.user_id, --default value from the login dialogue
  tbl_entities.entity_id, -- automatically created if NULL
  tbl_entities.timestamp_creation, --a
  tbl_entities.timestamp_edit, --a
  tbl_entities.user_edit, -- the fronted should insert the current users user_id on update
  tbl_entities.classes_uid, -- the frontend should insert the uid of the physical thing class on insert (=12)
  tbl_entities.entity_name_uri, --v e
  tbl_entities.entity_description, -- v e 
  tbl_entities.start_time_abs, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.entity_remarks, 
  tbl_entities.end_time_abs, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.start_time_text, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.end_time_text, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.dim_width, -- v e
  tbl_entities.dim_length, -- v e
  tbl_entities.dim_height, -- v e
  tbl_entities.dim_thickness, -- v e
  tbl_entities.dim_diameter, -- v e
  tbl_entities.dim_units, -- v e
  tbl_entities.dim_degrees, -- v e 
  tbl_entities.x_lon_easting, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.y_lat_northing, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.srid_epsg, -- e 
			  -- The frontend should insert the parents Value on default.
                          -- If NULL then the frontend should automatically insert a Default SRID Number defined in the global settings
                          -- If necessary the user should be able to select another SRID by its name or number in a dialog that saves the chosen srid's number in the srid field of the current dataset)
                          -- In the GUI a Lookup Field should show the SRIDS name
  tbl_entities.geom, -- a
  tbl_entities.entity_type, -- e (In the GUI the user should be able to select the type by its name in a hierarchical organized Select-Dialogue that saves the chosen type's uid in the entity_type field of the current dataset)
  types_all_tree.name_path  -- v
FROM 
  openatlas.types_all_tree, 
  openatlas.tbl_entities
WHERE 
  tbl_entities.entity_type = types_all_tree.id AND
  tbl_entities.classes_uid = 12 AND -- entity has to be a physical thing
  types_all_tree.name_path LIKE '%> Feature%'; -- entity's type must be Feature or Subtype



--stratigraphical unit: physical things of which a feature is composed of like backfillings, skeletons, deposits...
  --  to document: Name, description, type, dating, center coordinates, dimensions
  --  defined by: Cidoc Class Nr = E018/physical thing AND Type = Stratigraphical Unit (or Subtype of Stratigraphical Unit)

CREATE OR REPLACE VIEW openatlas.stratigraphical_units AS  
SELECT 
  tbl_entities.uid, -- a
  tbl_entities.user_id, --default value from the login dialogue
  tbl_entities.entity_id, -- automatically created if NULL
  tbl_entities.timestamp_creation, --a
  tbl_entities.timestamp_edit, --a
  tbl_entities.user_edit, -- the fronted should insert the current users user_id on update
  tbl_entities.classes_uid, -- the frontend should insert the uid of the physical thing class on insert (=12)
  tbl_entities.entity_name_uri, --v e
  tbl_entities.entity_description, -- v e 
  tbl_entities.start_time_abs, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.entity_remarks, 
  tbl_entities.end_time_abs, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.start_time_text, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.end_time_text, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.dim_width, -- v e
  tbl_entities.dim_length, -- v e
  tbl_entities.dim_height, -- v e
  tbl_entities.dim_thickness, -- v e
  tbl_entities.dim_diameter, -- v e
  tbl_entities.dim_units, -- v e
  tbl_entities.dim_weight, -- v e
  tbl_entities.dim_units_weight, -- v e
  tbl_entities.dim_degrees, -- v e 
  tbl_entities.x_lon_easting, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.y_lat_northing, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.srid_epsg, -- e 
			  -- The frontend should insert the parents Value on default.
                          -- If NULL then the frontend should automatically insert a Default SRID Number defined in the global settings
                          -- If necessary the user should be able to select another SRID by its name or number in a dialog that saves the chosen srid's number in the srid field of the current dataset)
                          -- In the GUI a Lookup Field should show the SRIDS name
  tbl_entities.geom, -- a
  tbl_entities.entity_type, -- e (In the GUI the user should be able to select the type by its name in a hierarchical organized Select-Dialogue that saves the chosen type's uid in the entity_type field of the current dataset)
  types_all_tree.name_path  -- v
FROM 
  openatlas.types_all_tree, 
  openatlas.tbl_entities
WHERE 
  tbl_entities.entity_type = types_all_tree.id AND
  tbl_entities.classes_uid = 12 AND -- entity has to be a physical thing
  types_all_tree.name_path LIKE '%> Stratigraphical Unit%'; -- entity's type must be Stratigraphical Unit or Subtype


--Finds: moveable physical things that were/are contained/found in stratigraphical units like pots, weapons, glass, tools...
  --  to document: Name, description, type, dating, center coordinates, dimensions
  --  defined by: Cidoc Class Nr = E019/physical object AND Type = Finds (or Subtype of Finds)

CREATE OR REPLACE VIEW openatlas.finds AS  
SELECT 
  tbl_entities.uid, -- a
  tbl_entities.user_id, --default value from the login dialogue
  tbl_entities.entity_id, -- automatically created if NULL
  tbl_entities.timestamp_creation, --a
  tbl_entities.timestamp_edit, --a
  tbl_entities.user_edit, -- the fronted should insert the current users user_id on update
  tbl_entities.classes_uid, -- the frontend should insert the uid of the physical object class on insert (=15)
  tbl_entities.entity_name_uri, --v e
  tbl_entities.entity_description, -- v e 
  tbl_entities.start_time_abs, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.entity_remarks, 
  tbl_entities.end_time_abs, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.start_time_text, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.end_time_text, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.dim_width, -- v e
  tbl_entities.dim_length, -- v e
  tbl_entities.dim_height, -- v e
  tbl_entities.dim_thickness, -- v e
  tbl_entities.dim_diameter, -- v e
  tbl_entities.dim_units, -- v e
  tbl_entities.dim_weight, -- v e
  tbl_entities.dim_units_weight, -- v e
  tbl_entities.dim_degrees, -- v e 
  tbl_entities.x_lon_easting, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.y_lat_northing, -- v e (The frontend should insert the parents Value on default)
  tbl_entities.srid_epsg, -- e 
			  -- The frontend should insert the parents Value on default.
                          -- If NULL then the frontend should automatically insert a Default SRID Number defined in the global settings
                          -- If necessary the user should be able to select another SRID by its name or number in a dialog that saves the chosen srid's number in the srid field of the current dataset)
                          -- In the GUI a Lookup Field should show the SRIDS name
  tbl_entities.geom, -- a
  tbl_entities.entity_type, -- e (In the GUI the user should be able to select the type by its name in a hierarchical organized Select-Dialogue that saves the chosen type's uid in the entity_type field of the current dataset)
  types_all_tree.name_path  -- v
FROM 
  openatlas.types_all_tree, 
  openatlas.tbl_entities
WHERE 
  tbl_entities.entity_type = types_all_tree.id AND
  tbl_entities.classes_uid = 15 AND -- entity has to be a physical thing
  types_all_tree.name_path LIKE '%> Finds%'; -- entity's type must be Finds or Subtype



--Bibliography: Literature, papers, reports, charters, documents ecc. that refer to resp. document other entities
  --  to document: Name, description, type, year, center coordinates, dimensions
  --  defined by: Cidoc Class Nr = E031/document AND Type = Text (or Subtype of Text)

CREATE OR REPLACE VIEW openatlas.texts AS  
SELECT 
  tbl_entities.uid, -- a
  tbl_entities.user_id, --default value from the login dialogue
  tbl_entities.entity_id, -- automatically created if NULL (= Jabref's id if bibliography is imported from/linked to Jabref)
  tbl_entities.timestamp_creation, --a
  tbl_entities.timestamp_edit, --a
  tbl_entities.user_edit, -- the fronted should insert the current users user_id on update
  tbl_entities.classes_uid, -- the frontend should insert the uid of the document class on insert (=11)
  tbl_entities.entity_name_uri, -- v e (Authors Name + Year)
  tbl_entities.entity_description, -- v e (Citation of document)
  tbl_entities.start_time_abs, -- v e (Year of Publishing) 
  tbl_entities.entity_remarks, 
  tbl_entities.entity_type, -- e (In the GUI the user should be able to select the type by its name in a hierarchical organized Select-Dialogue that saves the chosen type's uid in the entity_type field of the current dataset)
  types_all_tree.name AS type_name, 
  types_all_tree.name_path  -- v
FROM 
  openatlas.types_all_tree, 
  openatlas.tbl_entities
WHERE 
  tbl_entities.entity_type = types_all_tree.id AND
  tbl_entities.classes_uid = 11 AND -- entity has to be a document
  types_all_tree.name_path LIKE '%> Text%'; -- entity's type must be Text or Subtype
  
  
 
--Evidence Links: Archeological units like sites, features, stratigraphical_units and finds that are linked to entities of Class E55 (type) that are subtypes (has broader term - P127) of "Evidence"
     
 CREATE OR REPLACE VIEW openatlas.links_evidence AS 
SELECT 
   openatlas.tbl_links.links_entity_uid_from, -- Site/Feature etc. that is known by a certain type of evidence
   openatlas.tbl_links.links_cidoc_number_direction, 
   openatlas.tbl_links.links_entity_uid_to, -- Evidence by which the site is known
   openatlas.tbl_links.links_creator,
   openatlas.tbl_links.links_uid,
   openatlas.types_all_tree.name_path, -- name path contains *Evidence
   openatlas.types_all_tree.name,
   openatlas.types_all_tree.path
FROM (openatlas.tbl_links INNER JOIN openatlas.tbl_entities ON openatlas.tbl_links.links_entity_uid_to = openatlas.tbl_entities.uid) 
 INNER JOIN openatlas.types_all_tree ON openatlas.tbl_entities.uid = openatlas.types_all_tree.id
 WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=1) AND ((openatlas.types_all_tree.name_path) Like '%> Evidence >%'));


--Archeological_Parent Links: Archeological units like sites, features and stratigraphical_units (E18) that are parents (is composed of - p46a) of other archeological units
     
 CREATE OR REPLACE VIEW openatlas.links_parents_arch AS 
SELECT 
   openatlas.tbl_links.links_entity_uid_from, --archeological parent like a certain Site
   openatlas.tbl_links.links_cidoc_number_direction, -- P46a
   openatlas.tbl_links.links_entity_uid_to, -- archeological child like a certain feature from a certain site
   openatlas.tbl_links.links_creator,
   openatlas.tbl_links.links_uid,
   openatlas.tbl_links.links_annotation,
   openatlas.types_all_tree.name_path,
   openatlas.types_all_tree.name,
   openatlas.types_all_tree.path,
   openatlas.tbl_entities.entity_name_uri,
   openatlas.tbl_entities.entity_description 
FROM openatlas.tbl_links 
 INNER JOIN (openatlas.tbl_entities INNER JOIN openatlas.types_all_tree ON openatlas.tbl_entities.entity_type=openatlas.types_all_tree.id)
 ON openatlas.tbl_links.links_entity_uid_to=openatlas.tbl_entities.uid 
 WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=11)) 
 ORDER BY openatlas.tbl_entities.entity_name_uri, openatlas.tbl_entities.uid; 
 

 
 
 --Image Links: Entitites (e.g. sites, features and stratigraphical_units, finds etc.) that are documented (P070b) in images (E31 entities that have the type image or subtype)
     
 CREATE OR REPLACE VIEW openatlas.links_images AS  
SELECT 
   openatlas.types_all_tree.name_path, -- must contain *> Image *
   openatlas.tbl_links.links_entity_uid_from, -- entity that is documented by an image
   openatlas.tbl_links.links_entity_uid_to, -- image that shows the entity
   openatlas.tbl_links.links_cidoc_number_direction, -- is documented in (P70b)
   openatlas.tbl_entities.uid, 
   openatlas.tbl_entities.entity_id, 
   openatlas.tbl_entities.entity_name_uri, 
   openatlas.tbl_links.links_uid
FROM openatlas.tbl_links 
INNER JOIN (openatlas.tbl_entities INNER JOIN openatlas.types_all_tree ON openatlas.tbl_entities.entity_type = openatlas.types_all_tree.id)
ON openatlas.tbl_links.links_entity_uid_to = openatlas.tbl_entities.uid
WHERE (((openatlas.types_all_tree.name_path) Like '%> Image %'))
ORDER BY openatlas.tbl_entities.uid;



 --Age Links: Burials/Skeletons that have a certain type (P002a) of age (type - E55, Subtype of "Age >"
     
 CREATE OR REPLACE VIEW openatlas.links_age AS
SELECT 
   openatlas.tbl_links.links_entity_uid_from, 
   openatlas.tbl_links.links_cidoc_number_direction, 
   openatlas.tbl_links.links_entity_uid_to, 
   openatlas.tbl_links.links_creator, 
   openatlas.tbl_links.links_uid, 
   openatlas.types_all_tree.name_path, 
   openatlas.types_all_tree.name, 
   openatlas.types_all_tree.path, 
   openatlas.tbl_links.links_annotation 
 FROM openatlas.tbl_links 
   INNER JOIN (openatlas.tbl_entities INNER JOIN openatlas.types_all_tree ON openatlas.tbl_entities.uid=openatlas.types_all_tree.id)
   ON openatlas.tbl_links.links_entity_uid_to=openatlas.tbl_entities.uid 
   WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=1) AND ((openatlas.types_all_tree.name_path) Like '%Age >%'));


--Sex Links: Burials/Skeletons that have a certain type (P002a) of sex (type - E55, Subtype of "Sex >"
     
 CREATE OR REPLACE VIEW openatlas.links_sex AS
SELECT 
   openatlas.tbl_links.links_entity_uid_from, 
   openatlas.tbl_links.links_cidoc_number_direction, 
   openatlas.tbl_links.links_entity_uid_to, 
   openatlas.tbl_links.links_creator, 
   openatlas.tbl_links.links_uid, 
   openatlas.types_all_tree.name_path, 
   openatlas.types_all_tree.name, 
   openatlas.types_all_tree.path, 
   openatlas.tbl_links.links_annotation 
 FROM openatlas.tbl_links 
   INNER JOIN (openatlas.tbl_entities INNER JOIN openatlas.types_all_tree ON openatlas.tbl_entities.uid=openatlas.types_all_tree.id)
   ON openatlas.tbl_links.links_entity_uid_to=openatlas.tbl_entities.uid 
   WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=1) AND ((openatlas.types_all_tree.name_path) Like '%Sex >%'));
   
   

--Bibliography Links: Entitites (e.g. sites, features and stratigraphical_units, finds, images etc.) that are documented (P070b) in texts (E31 entities that have a subtype of text)
     
 CREATE OR REPLACE VIEW openatlas.links_bibliography AS   
SELECT 
  openatlas.tbl_links.links_entity_uid_from, 
  openatlas.tbl_links.links_cidoc_number_direction, 
  openatlas.tbl_links.links_entity_uid_to, 
  openatlas.tbl_links.links_creator, 
  openatlas.tbl_links.links_uid, 
  openatlas.types_all_tree.name_path, 
  openatlas.types_all_tree.name, 
  openatlas.types_all_tree.path, 
  openatlas.tbl_entities.entity_name_uri, 
  openatlas.tbl_entities.entity_description, 
  openatlas.tbl_links.links_annotation
FROM openatlas.tbl_links 
   INNER JOIN (openatlas.tbl_entities INNER JOIN openatlas.types_all_tree ON openatlas.tbl_entities.entity_type = openatlas.types_all_tree.id)
    ON openatlas.tbl_links.links_entity_uid_to = openatlas.tbl_entities.uid
WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=4) AND ((openatlas.types_all_tree.name_path) Like '%> Text >%'))
ORDER BY openatlas.tbl_entities.entity_name_uri, openatlas.tbl_entities.uid;



--Chronological Links: Archeological units like sites, features, stratigraphical_units and finds that are linked (P86a) to entities of Class E52 (timespan)
     
 CREATE OR REPLACE VIEW openatlas.links_chronological AS 

SELECT 
   openatlas.tbl_links.links_entity_uid_from, 
   openatlas.tbl_links.links_cidoc_number_direction, 
   openatlas.tbl_links.links_entity_uid_to, 
   openatlas.tbl_links.links_creator, 
   openatlas.tbl_links.links_uid, 
   openatlas.chronological_period_all_tree.name_path, 
   openatlas.chronological_period_all_tree.name, 
   openatlas.chronological_period_all_tree.path 
 FROM openatlas.tbl_links 
    INNER JOIN openatlas.chronological_period_all_tree ON openatlas.tbl_links.links_entity_uid_to=openatlas.chronological_period_all_tree.id 
    WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=13)); 
    
    
--cultural Links: Archeological units like sites, features, stratigraphical_units and finds that are linked (P10a) to entities of Class E4 (period)
CREATE OR REPLACE VIEW openatlas.links_cultural AS 
SELECT 
  openatlas.tbl_links.links_entity_uid_from, 
  openatlas.tbl_links.links_cidoc_number_direction, 
  openatlas.tbl_links.links_entity_uid_to, 
  openatlas.tbl_links.links_creator, 
  openatlas.tbl_links.links_uid, 
  openatlas.cultural_period_all_tree.name_path, 
  openatlas.cultural_period_all_tree.name, 
  openatlas.cultural_period_all_tree.path 
FROM openatlas.tbl_links 
 INNER JOIN openatlas.cultural_period_all_tree ON openatlas.tbl_links.links_entity_uid_to=openatlas.cultural_period_all_tree.id 
 WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=9)); 
   



--grave construction Links: Graves that are (P2 has tyoe) to entities of Class E55 (type) that are subtypes of 'Grave Construction'
CREATE OR REPLACE VIEW openatlas.links_graveconstruction AS    
SELECT 
   openatlas.tbl_links.links_entity_uid_from, 
   openatlas.tbl_links.links_cidoc_number_direction, 
   openatlas.tbl_links.links_entity_uid_to, 
   openatlas.tbl_links.links_creator, 
   openatlas.tbl_links.links_uid, 
   openatlas.types_all_tree.name_path, 
   openatlas.types_all_tree.name, 
   openatlas.types_all_tree.path, 
   openatlas.tbl_links.links_annotation
 FROM openatlas.tbl_links 
    INNER JOIN (openatlas.tbl_entities 
       INNER JOIN openatlas.types_all_tree 
       ON openatlas.tbl_entities.uid = openatlas.types_all_tree.id) 
    ON openatlas.tbl_links.links_entity_uid_to = openatlas.tbl_entities.uid
 WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=1) AND ((openatlas.types_all_tree.name_path) Like '%Grave Construction%'));

 
 
--grave shape Links: Graves that are linked (P2 has type) to entities of Class E55 (type) that are subtypes of 'Grave Shape'
CREATE OR REPLACE VIEW openatlas.links_graveshape AS    
SELECT 
   openatlas.tbl_links.links_entity_uid_from, 
   openatlas.tbl_links.links_cidoc_number_direction, 
   openatlas.tbl_links.links_entity_uid_to, 
   openatlas.tbl_links.links_creator, 
   openatlas.tbl_links.links_uid, 
   openatlas.types_all_tree.name_path, 
   openatlas.types_all_tree.name, 
   openatlas.types_all_tree.path, 
   openatlas.tbl_links.links_annotation
 FROM openatlas.tbl_links 
    INNER JOIN (openatlas.tbl_entities 
       INNER JOIN openatlas.types_all_tree 
       ON openatlas.tbl_entities.uid = openatlas.types_all_tree.id) 
    ON openatlas.tbl_links.links_entity_uid_to = openatlas.tbl_entities.uid
 WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=1) AND ((openatlas.types_all_tree.name_path) Like '%Grave Shape%'));
 
 
 --Material Links: archeological units that have a certain type (P002a) of material (E57, Subtype of "Material >"
     
 CREATE OR REPLACE VIEW openatlas.links_material AS
SELECT 
   openatlas.tbl_links.links_entity_uid_from, 
   openatlas.tbl_links.links_cidoc_number_direction, 
   openatlas.tbl_links.links_entity_uid_to, 
   openatlas.tbl_links.links_creator, 
   openatlas.tbl_links.links_uid, 
   openatlas.types_all_tree.name_path, 
   openatlas.types_all_tree.name, 
   openatlas.types_all_tree.path, 
   openatlas.tbl_links.links_annotation 
 FROM openatlas.tbl_links 
   INNER JOIN (openatlas.tbl_entities INNER JOIN openatlas.types_all_tree ON openatlas.tbl_entities.uid=openatlas.types_all_tree.id)
   ON openatlas.tbl_links.links_entity_uid_to=openatlas.tbl_entities.uid 
   WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=1) AND ((openatlas.types_all_tree.name_path) Like '%Material >%'));

   
--Place Links: archeological units that are located within (P053a) places (E53)
     
 CREATE OR REPLACE VIEW openatlas.links_places AS
SELECT 
    openatlas.tbl_links.links_entity_uid_from, 
    openatlas.tbl_links.links_cidoc_number_direction, 
    openatlas.tbl_links.links_entity_uid_to, 
    openatlas.tbl_links.links_creator, 
    openatlas.tbl_links.links_uid, 
    openatlas.place_all_tree.name_path, 
    openatlas.place_all_tree.name, 
    openatlas.place_all_tree.path, 
    tbl_entities_1.entity_name_uri AS type_name, 
    openatlas.tbl_links.links_annotation
  FROM (openatlas.tbl_links 
     INNER JOIN openatlas.place_all_tree ON openatlas.tbl_links.links_entity_uid_to = openatlas.place_all_tree.id) 
     INNER JOIN (openatlas.tbl_entities INNER JOIN openatlas.tbl_entities AS tbl_entities_1 ON openatlas.tbl_entities.entity_type = tbl_entities_1.uid) 
     ON openatlas.place_all_tree.id = openatlas.tbl_entities.uid
  WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=15) AND ((openatlas.place_all_tree.name_path) Like '%Europe%'));   
   

   
--Rights Links: entities that are that are subject to (P104a) certain rights (E30)
     
 CREATE OR REPLACE VIEW openatlas.links_rights AS
SELECT 
    openatlas.tbl_links.links_entity_uid_from, 
    openatlas.tbl_links.links_cidoc_number_direction, 
    openatlas.tbl_links.links_entity_uid_to, 
    openatlas.tbl_links.links_creator, 
    openatlas.tbl_links.links_uid, 
    openatlas.tbl_entities.classes_uid, 
    openatlas.tbl_entities.entity_name_uri, 
    openatlas.tbl_entities.entity_description, 
    openatlas.tbl_entities.entity_id
  FROM openatlas.tbl_links 
    INNER JOIN openatlas.tbl_entities ON openatlas.tbl_links.links_entity_uid_to = openatlas.tbl_entities.uid
  WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=17) AND ((openatlas.tbl_entities.classes_uid)=21));
  

  
  --Rightsholder Links: entities whose rights are held (P105a) by certain actors (E39)
     
 CREATE OR REPLACE VIEW openatlas.links_rightsholder AS  
SELECT 
   openatlas.tbl_links.links_entity_uid_from, 
   openatlas.tbl_links.links_cidoc_number_direction, 
   openatlas.tbl_links.links_entity_uid_to, 
   openatlas.tbl_links.links_creator, 
   openatlas.tbl_links.links_uid, 
   openatlas.tbl_entities.classes_uid, 
   openatlas.tbl_entities.entity_name_uri, 
   openatlas.tbl_entities.entity_description, 
   openatlas.tbl_entities.entity_id
  FROM openatlas.tbl_links 
     INNER JOIN openatlas.tbl_entities ON openatlas.tbl_links.links_entity_uid_to = openatlas.tbl_entities.uid
  WHERE (((openatlas.tbl_links.links_cidoc_number_direction)=19));



  

GRANT SELECT ON ALL TABLES IN SCHEMA public TO public;
GRANT SELECT ON ALL TABLES IN SCHEMA openatlas TO public;


--GRANT ALL ON SCHEMA public TO openatla_jansaviktor; -- replace name and privileges if necessary
--GRANT ALL ON ALL TABLES IN SCHEMA openatlas TO openatla_jansaviktor; -- replace name and privileges if necessary
--GRANT ALL ON SCHEMA public TO openatla_jansaviktor; -- replace name and privileges if necessary
--GRANT ALL ON ALL TABLES IN SCHEMA openatlas TO openatla_jansaviktor; -- replace name and privileges if necessary

  


