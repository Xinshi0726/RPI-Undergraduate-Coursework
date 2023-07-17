DROP FUNCTION IF EXISTS recommendation(time,time,varchar,int,float,float,float);
DROP FUNCTION IF EXISTS recommendation_helper(time,time,varchar,int,float,float,float);

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

DROP TABLE IF EXISTS songsimilarity;

CREATE TABLE songsimilarity(
    songid       int
    , songname   text
    , artistname text
    , songscore  float
) ;
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
    myrow songsimilarity%rowtype ;
BEGIN
    --drop tables used below
    DROP TABLE IF EXISTS songsplayed;
    DROP TABLE IF EXISTS unplayed;
    DROP TABLE IF EXISTS genrescore;
    DROP TABLE IF EXISTS genresimilarity;
    DROP TABLE IF EXISTS avgbilboard;
    DROP TABLE IF EXISTS avgbilboardplayed;
    DROP TABLE IF EXISTS rank_similarity;
    DROP TABLE IF EXISTS avgfeatures;
    CREATE TABLE songsplayed (
        id int
    );
    INSERT INTO songsplayed
        SELECT DISTINCT p.songid
        FROM playedonradio p
        WHERE p.station = inputstation AND p.playedtime::time BETWEEN fromtime AND totime;
    CREATE TABLE genrescore(
      genre VARCHAR,
      score INT
    );
    INSERT INTO genrescore
        SELECT sg.genre, COUNT(sp.id)
        FROM song_genre sg LEFT JOIN songsplayed sp on sg.songid = sp.id
        GROUP BY sg.genre;
    CREATE TABLE avgbilboard(
        id INT,
        decade varchar,
        averagerank FLOAT
    );
    CREATE TABLE unplayed(
        id INT
    );
    INSERT INTO unplayed
        SELECT DISTINCT s.id
        FROM songs s
        WHERE s.id NOT IN (SELECT id FROM songsplayed);
    INSERT INTO avgbilboard
        SELECT
            s.id,
            s.decade,
            AVG(b.rank)
        FROM songs s
        LEFT JOIN bilboard b ON s.id = b.songid
        WHERE s.id IN (SELECT songid FROM unplayed)
        GROUP BY s.id,s.decade;

    CREATE TABLE avgbilboardplayed(
        decade VARCHAR,
        avgplayed FLOAT
    );
    INSERT INTO avgbilboardplayed
    SELECT
        s.decade,
        AVG(ab.averagerank)
    FROM songs s JOIN songsplayed sp on sp.id = s.id
    LEFT JOIN avgbilboard ab on s.id = ab.id
    group by s.decade;
    CREATE TABLE avgfeatures(
        avgenergy FLOAT,
        avglivness FLOAT,
        avgacousticness FLOAT
    );
    --store all needed average values into this table
    INSERT INTO avgfeatures
    SELECT
        AVG(s.energy) as avgenergy,
        AVG(s.liveness) as avgliveness,
        AVG(s.acousticness) as avgacousticness
    FROM songs s
    WHERE s.id IN (SELECT id FROM songsplayed);
    CREATE TABLE genresimilarity(
        id INT,
        score FLOAT
    );
    INSERT INTO genresimilarity
    SELECT up.id,
           SUM(gs.score)
    FROM unplayed up LEFT JOIN song_genre sg ON up.id = sg.songid
        LEFT JOIN genrescore gs on sg.genre = gs.genre
    GROUP BY up.id;
    CREATE TABLE rank_similarity(
        id INT,
        score FLOAT
    );
    INSERT INTO rank_similarity
    SELECT
            avgr.id,
            CASE
                WHEN avgr.decade IS NULL THEN 0
                ELSE
            (CASE
                WHEN (avgr.averagerank is NULL OR avgdr.avgplayed is NULL OR avgr.averagerank - avgdr .avgplayed = 0) THEN 0
                ELSE 1 / ABS(avgr.averagerank - avgdr.avgplayed)
                END)
            END as rs
            FROM avgbilboard avgr LEFT JOIN avgbilboardplayed avgdr ON avgr. decade = avgdr. decade;
    --return all outputs
    FOR myrow in SELECT s.id as songid, s.name as songname, a.name as artistname, W1*(CASE WHEN gs.score is NULL THEN 0 ELSE gs.score END)+
                                                                                 W2*(case WHEN ranks.score is NULL THEN 0 ELSE ranks.score END)+
                                                                                 w3*(CASE WHEN s.energy is NULL OR s.liveness is NULL OR s.acousticness is NULL THEN 0
                                                                                     ELSE 1/(abs(s.energy-SUM(af.avgenergy))+abs(s.liveness-SUM(af.avglivness))+abs(s.acousticness-SUM(af.avgacousticness)))END)
                                                                                as songscore
                FROM
                    songs s JOIN artists a ON s.artistid = a.id
                    LEFT JOIN genresimilarity gs ON s.id = gs.id
                    LEFT JOIN rank_similarity ranks ON s.id = ranks.id
                    ,avgfeatures af
                WHERE s.id in (SELECT id from unplayed)
                GROUP BY s.id, s.name, a.name, gs.score, ranks.score
                ORDER BY songscore DESC,s.name ASC
                LIMIT topk
    LOOP
        RETURN NEXT myrow;
        end loop;

END;
$$;

--- stations:  mai george sound rock breeze edge magic more

select * from recommendation_helper(time '08:00',time '10:00', 'mai', 10,1,0,0);
select * from recommendation(time '08:00',time '10:00', 'mai', 10,1,0,0);

select * from recommendation_helper(time '08:00',time '10:00', 'mai', 10,0,1,0);
select * from recommendation(time '08:00',time '10:00', 'mai', 10,0,1,0);

select * from recommendation_helper(time '08:00',time '10:00', 'mai', 10,0,0,1);
select * from recommendation(time '08:00',time '10:00', 'mai', 10,0,0,1);

select * from recommendation_helper(time '08:00',time '10:00', 'mai', 20,0.05,1,1);
select * from recommendation(time '08:00',time '10:00', 'mai', 20,0.05,1,1);

select * from recommendation_helper(time '20:00',time '22:00', 'mai', 10,1,0,0);
select * from recommendation(time '20:00',time '22:00', 'mai', 10,1,0,0);

select * from recommendation_helper(time '20:00',time '22:00', 'mai', 10,0,1,0);
select * from recommendation(time '20:00',time '22:00', 'mai', 10,0,1,0);

select * from recommendation_helper(time '20:00',time '22:00', 'mai', 10,0,0,1);
select * from recommendation(time '20:00',time '22:00', 'mai', 10,0,0,1);

select * from recommendation_helper(time '20:00',time '22:00', 'mai', 20,0.05,1,1);
select * from recommendation(time '20:00',time '22:00', 'mai', 20,0.05,1,1);

select * from recommendation_helper(time '08:00',time '10:00', 'more', 10,1,0,0);
select * from recommendation(time '08:00',time '10:00', 'more', 10,1,0,0);

select * from recommendation_helper(time '08:00',time '10:00', 'more', 10,0,1,0);
select * from recommendation(time '08:00',time '10:00', 'more', 10,0,1,0);

select * from recommendation_helper(time '08:00',time '10:00', 'more', 10,0,0,1);
select * from recommendation(time '08:00',time '10:00', 'more', 10,0,0,1);

select * from recommendation_helper(time '08:00',time '10:00', 'more', 20,0.05,1,1);
select * from recommendation(time '08:00',time '10:00', 'more', 20,0.05,1,1);

select * from recommendation_helper(time '20:00',time '22:00', 'edge', 10,1,0,0);
select * from recommendation(time '20:00',time '22:00', 'edge', 10,1,0,0);

select * from recommendation_helper(time '20:00',time '22:00', 'edge', 10,0,1,0);
select * from recommendation(time '20:00',time '22:00', 'edge', 10,0,1,0);

select * from recommendation_helper(time '20:00',time '22:00', 'edge', 10,0,0,1);
select * from recommendation(time '20:00',time '22:00', 'edge', 10,0,0,1);

select * from recommendation_helper(time '20:00',time '22:00', 'edge', 20,0.05,1,1);
select * from recommendation(time '20:00',time '22:00', 'edge', 20,0.05,1,1);
