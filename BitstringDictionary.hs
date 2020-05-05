import Data.Char
type Entry = (Int, Char)
type Dictionary = [Entry]
dictionary :: Dictionary
dictionary = zip [0..] (['A'..'Z'] ++ ['a'..'z'] ++ ['0'..'9'] ++ ['+','/'])


pad :: ([a] -> [a] -> [a]) -> a -> Int -> [a] -> [a]
pad f a x b
   | x > (length b) = f (replicate (x - length b) a) b
   | otherwise = b
padLeft  = pad (\p -> (p ++))
padRight = pad (\p -> (++ p))


type BitString = [Int]
toBitString :: Int -> BitString
toBitString 0 = []
toBitString n
            | n `mod` 2 == 1 = toBitString (n `div` 2) ++ [1]
            | n `mod` 2 == 0 = toBitString (n `div` 2) ++ [0]
			

fromBitString :: BitString -> Int
fromBitString [] = 0
fromBitString (x:xs)
              | x == 1 = 2^(length xs) + fromBitString xs
              | x == 0 = fromBitString xs
			  

toBinary :: String -> BitString
toBinary []=[]
toBinary (x:xs) = ( padLeft 0 8(toBitString (ord x)) ) ++ toBinary xs


chunksOf :: Int -> [a] -> [[a]]
chunksOf n []
         | n <= 0 = error "chunksOf: Invalid chunk length"
         | otherwise = []
chunksOf n l
         | n <= 0 = error "chunksOf: Invalid chunk length"
         | otherwise = (take n l):(chunksOf n (drop n l))
		 
		
findFirst :: (a -> Bool) -> [a] -> a
findFirst p [] = error "findFirst: No such entry"
findFirst p (x:xs)
            | any p (x:xs) = head (filter p (x:xs))
            | otherwise = error "findFirst: No such entry"


findChar :: Dictionary -> BitString -> Char
findChar dictionary b = snd ( findFirst ( == (dictionary !! fromBitString b) ) dictionary )

translate :: Dictionary -> String -> String
translate dictionary "" = ""
translate dictionary s
          | (length s) `mod` 3 == 0 = (map (findChar dictionary) (chunksOf 6 ((toBinary s))))
          | otherwise =  (map (findChar dictionary) (chunksOf 6 (padRight 0 x (toBinary s))))
          where x = ((length (toBinary s) + 6) `div` 6)*6
		  
		  
encode :: Dictionary -> String -> String		  
encode dictionary s
       | (length a) `mod` 4 == 0 = a
       | otherwise = padRight '=' x a
       where x = ((length a + 4) `div` 4)*4
             a = translate dictionary s
			 
			 
findCode :: Dictionary -> Char -> BitString			 
findCode dictionary a = toBitString (head (map (fst)(filter (\(k,z)->z==a) dictionary)))



