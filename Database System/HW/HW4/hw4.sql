SELECT a.name AS artistname, COUNT(DISTINCT pr.songid) AS numsongs, MIN(r.position) AS bestpos
FROM artists a
JOIN rollingstonetop500 r ON a.id = r.artistid
JOIN songs s ON a.id = s.artistid
LEFT JOIN playedonradio pr ON s.id = pr.songid
GROUP BY a.name
HAVING MIN(r.position) <= 20
ORDER BY numsongs DESC, artistname ASC, bestpos DESC;

SELECT s.name AS songname, a.name AS artistname, COUNT(DISTINCT pr.id) AS numplayed
FROM songs s
LEFT JOIN artists a ON s.artistid = a.id
LEFT JOIN bilboard b ON s.id = b.songid
LEFT JOIN playedonradio pr ON s.id = pr.songid
WHERE s.loudness <= -11.5 AND b.rank BETWEEN 6 AND 50
GROUP BY s.name, a.name
ORDER BY numplayed DESC, songname ASC, artistname ASC;

SELECT s.name AS songname, a.name AS artistname, MIN(b.rank) AS minrank
FROM songs s
LEFT JOIN artists a ON s.artistid = a.id
LEFT JOIN bilboard b ON s.id = b.songid
LEFT JOIN spotify sp ON s.id = sp.songid
GROUP BY s.name, a.name
HAVING MAX(b.weeksonboard)>=30 AND COUNT(sp.songid) = 0
ORDER BY
    minrank ASC, songname ASC, artistname ASC;

SELECT s.name AS songname, a.name AS artistname, MIN(b.rank) AS minrank
FROM songs s
JOIN artists a ON s.artistid = a.id
JOIN bilboard b ON s.id = b.songid
AND s.id NOT IN (
    SELECT songid
    FROM spotify
)
GROUP BY s.id, s.name, a.name
HAVING MAX(b.weeksonboard) >= 30
ORDER BY minrank, songname, artistname;

SELECT s.name AS songname, a.name AS artistname, s.decade, CAST(s.duration_ms / 60000 AS integer) AS duration
FROM songs s
JOIN artists a ON s.artistid = a.id
WHERE CAST(s.duration_ms/ 60000 AS integer) = (
    SELECT CAST(MIN(duration_ms)/ 60000 AS integer)
    FROM songs
)
ORDER BY songname, artistname;

WITH total_streams AS (
    SELECT songid, SUM(streams) AS totalstreams
    FROM spotify
    GROUP BY songid
)
SELECT s.name AS songname, a.name AS artistname, TO_CHAR(ts.totalstreams, '999,999,999,999') AS totalstreams
FROM songs s
JOIN artists a ON s.artistid = a.id
JOIN total_streams ts ON s.id = ts.songid
JOIN (
    SELECT MAX(totalstreams) * 0.5 AS min_top_half
    FROM total_streams
) AS ss ON true
WHERE ts.totalstreams >= ss.min_top_half
ORDER BY songname, artistname;


SELECT DISTINCT a.name AS artistname
FROM artists a
LEFT JOIN songs s ON a.id = s.artistid
LEFT JOIN bilboard b ON s.id = b.songid
WHERE a.id IN (
    SELECT artistid
    FROM rollingstonetop500
    WHERE year >= 2004
)
AND b.rank <= 10
GROUP BY a.name
ORDER BY artistname;


SELECT p.station, COUNT(DISTINCT p.songid) AS numsongs
FROM playedonradio p
JOIN (
    SELECT songid, COUNT(DISTINCT station) AS num_stations
    FROM playedonradio
    GROUP BY songid
    HAVING COUNT(DISTINCT station) <= 2
) AS sc ON p.songid = sc.songid
WHERE p.station LIKE 'm%'
GROUP BY p.station
ORDER BY p.station;












