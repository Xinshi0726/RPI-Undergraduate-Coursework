DROP FUNCTION IF EXISTS recommendation(time,time,varchar,int,float,float,float);
DROP FUNCTION IF EXISTS recommendation_helper(time,time,varchar,int,float,float,float);
DROP TABLE IF EXISTS songsimilarity CASCADE ;
CREATE OR REPLACE FUNCTION
    recommendation_helper(fromtime time
               , totime time
               , inputstation varchar
               , topk int
	       , w1 float, w2 float, w3 float) RETURNS VARCHAR AS $$
BEGIN
   RETURN 'Inputs: '||fromtime::varchar(5)||' '||totime::varchar(5)||' / '||inputstation||' / '||topk::varchar||' / '||w1::varchar||' '||w2::varchar||' '||w3::varchar ;
END ;
$$ LANGUAGE plpgsql ;

CREATE TABLE songsimilarity (
    id        bigint,
    name      text,
    artist    text,
    songscore double precision
);

CREATE OR REPLACE FUNCTION recommendation(
    fromtime    time,
    totime      time,
    inputstation varchar,
    topk        integer,
    w1          double precision,
    w2          double precision,
    w3          double precision
) RETURNS SETOF songsimilarity
LANGUAGE plpgsql
AS $$
DECLARE
    rec songsimilarity;
BEGIN
    RETURN QUERY
    WITH songsplayed AS (
        SELECT DISTINCT p.songid
        FROM playedonradio p
        WHERE p.station = inputstation AND cast(p.playedtime::timestamp as time) BETWEEN fromtime AND totime
    ),
    songsNplayed AS (
        SELECT DISTINCT s.id
        FROM songs s
        WHERE s.id not in (SELECT*FROM songsplayed)
    ),
    genrescore AS (
        SELECT sg.genre, COUNT(sg.songid) as genre_score
        FROM song_genre sg
        WHERE sg.songid IN (SELECT songid FROM songsplayed)
        GROUP BY sg.genre
    ),
    genresimilarity AS (
      SELECT nsp.id, SUM(gs.genre_score) AS score
      FROM songsNplayed nsp
      LEFT JOIN  song_genre sg ON nsp.id = sg.songid
      LEFT JOIN genrescore gs ON gs.genre = sg.genre
      GROUP BY
          nsp.id
    ),
    avgbilboard AS (
        SELECT
            s.id,
            s.decade,
            AVG(b.rank) as avgrankplayed
        FROM songs s
        LEFT JOIN bilboard b ON s.id = b.songid
--         WHERE s.id IN (SELECT songid FROM songsplayed)
        GROUP BY s.id,s.decade
    ),
    avgbilboardplayed AS (
        SELECT
            s.decade,
            AVG(ar.avgrankplayed) as avgplayed
            FROM songs s
            JOIN songsplayed sp on sp.songid = s.id
            LEFT JOIN avgbilboard ar on ar.id = s.id
            GROUP BY s. decade
    ),
    ranksimiliarity AS (
        SELECT
            avgr.id,
            CASE
                WHEN avgr.decade IS NULL THEN 0
                ELSE
            (CASE
                WHEN (avgr.avgrankplayed is NULL OR avgdr .avgplayed is NULL OR avgr.avgrankplayed - avgdr .avgplayed = 0) THEN 0
                ELSE 1 / ABS(avgr.avgrankplayed - avgdr.avgplayed)
                END)
            END as rs
            FROM avgbilboard avgr LEFT JOIN avgbilboardplayed avgdr ON avgr. decade = avgdr. decade
    ),
    avgfeatures AS (
        SELECT
            AVG(s.energy) as avgenergy,
            AVG(s.liveness) as avgliveness,
            AVG(s.acousticness) as avgacousticness
        FROM songs s
        WHERE s.id IN (SELECT songid FROM songsplayed)
    ),
    songswithscore AS (
        SELECT
            s.id,
            s.name,
            a.name as artist,
            w1 * SUM(gs.genre_score) AS gs,
            w2 * COALESCE(1 / ABS(AVG(b.rank) - ab.avgrankplayed), 0) AS rs,
            w3 * COALESCE(1 / (ABS(s.energy - af.avgenergy) + ABS(s.liveness - af.avgliveness) + ABS(s.acousticness - af.avgacousticness)), 0) AS ss
        FROM songs s
        JOIN artists a ON s.artistid = a.id
        LEFT JOIN song_genre sg ON s.id = sg.songid
        LEFT JOIN genrescore gs ON sg.genre = gs.genre
        LEFT JOIN bilboard b ON s.id = b.songid
        LEFT JOIN avgbilboard ab ON s.decade = ab.decade
        LEFT JOIN avgfeatures af ON true
        WHERE s.id NOT IN (SELECT songid FROM songsplayed)
        GROUP BY s.id, a.name, ab.avgrankplayed, af.avgenergy, af.avgliveness, af.avgacousticness
    )
    SELECT
        id,
        name,
        artist,
        (gs + rs + ss) as songscore
    FROM songswithscore
    ORDER BY (gs + rs + ss) DESC, name
    LIMIT topk;
END;
$$;

select * from recommendation(time '08:00',time '10:00', 'mai', 10,1,0,0);