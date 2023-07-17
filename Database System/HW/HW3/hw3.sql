--Question 1
SELECT DISTINCT
ars.name
FROM
bilboard bb JOIN songs ss ON
bb.songid = ss.id JOIN artists ars ON
ss.artistid = ars.id
WHERE
bb.weeksonboard > 30
ORDER BY
ars.name ASC;

--Question 2
SELECT ars.name, ss.name, MAX(sp.streams)
FROM
artists ars LEFT JOIN rollingstonetop500 rls ON ars.id = rls.artistid LEFT JOIN
spotify sp JOIN songs ss ON sp.songid = ss.id ON ss.artistid = ars.id
WHERE
sp.streams > 100000 AND
rls.position <= 10
GROUP BY
ars.name,
ss.name
ORDER BY
ars.name ASC,
MAX(sp.streams) DESC;

--Question 3
SELECT ss.id AS songid, ss.name AS songname, ars.name AS artistname
FROM artists ars
LEFT JOIN songs ss ON ars.id = ss.artistid
LEFT JOIN bilboard bb ON ss.id = bb.songid
WHERE (bb.chartdate >= '2020-03-01' AND bb.chartdate <= '2020-05-31'
AND bb.weeksonboard >= 5) OR (ss.id IN (
    SELECT songid FROM playedonradio
    WHERE playedtime >= '2020-05-01' AND playedtime <= '2020-05-31'
    GROUP BY songid
    HAVING COUNT(*) >= 20
))
GROUP BY
ss.id,
ss.name,
ars.name
ORDER BY songname ASC, artistname ASC;

--Question 4
SELECT s.id AS songid, s.name AS songname, a.name AS artistname
FROM songs AS s
JOIN artists AS a ON s.artistid = a.id
RIGHT JOIN spotify AS sp ON s.id = sp.songid
WHERE (EXTRACT(DOW FROM sp.streamdate) = 0 OR EXTRACT(DOW FROM sp.streamdate) = 6)
EXCEPT
SELECT s.id AS songid, s.name AS songname, a.name AS artistname
FROM songs AS s
JOIN artists AS a ON s.artistid = a.id
RIGHT JOIN spotify AS sp ON s.id = sp.songid
WHERE (EXTRACT(DOW FROM sp.streamdate) = 1 OR EXTRACT(DOW FROM sp.streamdate) = 4)
GROUP BY
s.id,
sp.songid,
a.name
ORDER BY songname ASC, artistname ASC;


--Question 5
SELECT t.id AS songid, t.name AS songname, ars.name AS artistname
FROM
((SELECT songid
FROM playedonradio
INTERSECT
SELECT songid
FROM spotify
WHERE streams > 1000000
EXCEPT
SELECT songid
FROM bilboard)
AS prs
JOIN songs ss ON prs.songid = ss.id) AS t
JOIN artists ars ON t.artistid = ars.id
ORDER BY
t.name ASC,
ars.name ASC;

--Question 6
SELECT DISTINCT ars.id AS id, ars.name AS name, count(DISTINCT bb.songid) AS songsonbilboard, MIN(bb.peakrank) AS minibilboardrank
FROM (SELECT artistid
FROM rollingstonetop500
WHERE position <= 20) AS t
LEFT JOIN artists ars ON t.artistid = ars.id
LEFT JOIN songs ss ON ss.artistid = ars.id
LEFT JOIN bilboard bb ON ss.id = bb.songid
AND ss.name IS NOT NULL
GROUP BY
ars.id,
ars.name
ORDER BY
ars.name ASC;

--Question 7
--Question 1: Why use a left join on songs? Question 2:played in one month for 5 times or all months combined?
SELECT DISTINCT a.id, a.name
FROM
(SELECT temp.songid
FROM (SELECT songid, COUNT(playedtime) AS numplayed, COUNT(DISTINCT station) AS numstation
FROM playedonradio
WHERE playedtime >= '2020-05-01' AND playedtime <= '2020-07-31'
GROUP BY
songid) AS temp
WHERE temp.numplayed>=5 AND temp.numstation>=3) AS validartists
JOIN songs ss ON validartists.songid = ss.id JOIN rollingstonetop500 r on ss.artistid = r.artistid
JOIN artists a on r.artistid = a.id
ORDER BY
a.id ASC;

--Question 8
SELECT DISTINCT sg.genre
FROM bilboard bd JOIN song_genre sg on sg.songid = bd.songid
GROUP BY
sg.genre
HAVING
count(DISTINCT bd.songid)>=3 AND
MIN(bd.peakrank) > 3
ORDER BY
sg.genre ASC;

