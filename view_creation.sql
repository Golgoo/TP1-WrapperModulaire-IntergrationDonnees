USE maurice_gautier_inte_donnees_tp1;
CREATE OR REPLACE VIEW TouristeCinephile(arrondissement,
                                         tournage_titre,
                                         tournage_adresse,
                                         nb_cinemas,
                                         cinema_adr_1,
                                         cinema_adr_2,
                                         cinema_adr_3,
                                         has_velib,
                                         velib_adr_1,
                                         velib_adr_2,
                                         velib_adr_3,
                                         velib_adr_4,
                                         velib_adr_5,
                                         wifi_adr_1,
                                         wifi_adr_2,
                                         wifi_adr_3,
                                         wifi_adr_4
                                        )
AS SELECT t.ardt, t.titre, t.adresse,
(
 SELECT count(*)
 FROM cinemas c
 WHERE c.cp = t.ardt
),
(
  SELECT c.adresse
  FROM cinemas c
  WHERE c.cp = t.ardt LIMIT 1
),
(
  SELECT c.adresse
  FROM cinemas c
  WHERE c.cp = t.ardt LIMIT 1 OFFSET 1
),
(
  SELECT c.adresse
  FROM cinemas c
  WHERE c.cp = t.ardt LIMIT 1 OFFSET 2
),
(
  SELECT EXISTS(SELECT * FROM velib v WHERE v.cp = t.ardt)
),
(
  SELECT v.adress from velib v where v.cp = t.ardt LIMIT 1
),
(
  SELECT v.adress from velib v where v.cp = t.ardt LIMIT 1 OFFSET 1
),
(
  SELECT v.adress from velib v where v.cp = t.ardt LIMIT 1 OFFSET 2
),
(
  SELECT v.adress from velib v where v.cp = t.ardt LIMIT 1 OFFSET 3
),
(
  SELECT v.adress from velib v where v.cp = t.ardt LIMIT 1 OFFSET 4
),
(
  SELECT h.adress from hotspots h where h.cp = t.ardt LIMIT 1
),
(
  SELECT h.adress from hotspots h where h.cp = t.ardt LIMIT 1 OFFSET 1
),
(
  SELECT h.adress from hotspots h where h.cp = t.ardt LIMIT 1 OFFSET 2
),
(
  SELECT h.adress from hotspots h where h.cp = t.ardt LIMIT 1 OFFSET 3
)
FROM tournages t
;
