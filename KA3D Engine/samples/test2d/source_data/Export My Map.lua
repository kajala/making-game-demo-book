--
-- Exports 2D map from Mappy 2D editor
--
-- Install:
-- Copy this file to C:\Program Files\Mappy14\luascr directory
-- and add "lua01=Export My Map.lua" to MAPWIN.INI
--
-- Copyright (C) 2005 Jani Kajala
--

function writeProperty( out, name )
	local prop = mappy.getValue( mappy[name] )
	out:write( name .. "=" .. prop .. "\n" )
end

function writeBlockProperty( out, i, name, more )
	out:write( mappy.getBlockValue(i, mappy[name]) )
	if ( more ) then
		out:write( "," )
	end
end

function getValue( name )
	return mappy.getValue( mappy[name] )
end

function main ()
	local isok,asname = mappy.fileRequester (".", "My Map (*.mym)", "*.mym", mappy.MMB_SAVE)
	if isok == mappy.MMB_OK then
	
		if (not (string.sub (string.lower (asname), -4) == ".mym")) then
			asname = asname .. ".mym"
		end
		
		out = io.open( asname, "w" )
		out:write( "VERSION=1\n" );
		
		writeProperty( out, "MAPWIDTH" )
		writeProperty( out, "MAPHEIGHT" )
		writeProperty( out, "BLOCKDEPTH" )
		writeProperty( out, "BLOCKWIDTH" )
		writeProperty( out, "BLOCKHEIGHT" )
		writeProperty( out, "NUMBLOCKSTR" )
		writeProperty( out, "NUMBLOCKGFX" )

		out:write( "\n" )
		out:write( "BLOCKINFO={\n" )
		local i = 0
		while ( i < getValue("NUMBLOCKSTR") ) do
			out:write( "    " .. i .. ", " )
			writeBlockProperty( out, i, "BLKBG", 1 )
			writeBlockProperty( out, i, "BLKFG1", 1 )
			writeBlockProperty( out, i, "BLKFG2", 1 )
			writeBlockProperty( out, i, "BLKFG3", 1 )
			writeBlockProperty( out, i, "BLKUSER1", 1 )
			writeBlockProperty( out, i, "BLKUSER2", 1 )
			writeBlockProperty( out, i, "BLKUSER3", 1 )
			writeBlockProperty( out, i, "BLKUSER4", 1 )
			writeBlockProperty( out, i, "BLKUSER5", 1 )
			writeBlockProperty( out, i, "BLKUSER6", 1 )
			writeBlockProperty( out, i, "BLKUSER7", 1 )
			writeBlockProperty( out, i, "BLKFLAG1", 1 )
			writeBlockProperty( out, i, "BLKFLAG2", 1 )
			writeBlockProperty( out, i, "BLKFLAG3", 1 )
			writeBlockProperty( out, i, "BLKFLAG4", 1 )
			writeBlockProperty( out, i, "BLKFLAG5", 1 )
			writeBlockProperty( out, i, "BLKFLAG6", 1 )
			writeBlockProperty( out, i, "BLKFLAG7", 1 )
			writeBlockProperty( out, i, "BLKFLAG8", i+1 < getValue("NUMBLOCKSTR") )
			out:write( "\n" )
			
			i = i + 1
		end
		out:write( "}\n" )

		local blockbpp = getValue("BLOCKDEPTH")
		if ( blockbpp == 8 ) then
			out:write( "\n" )
			out:write( "PALETTE={\n" )
			local i = 0
			while ( i < 256 ) do
				out:write( "    " )
				local a,r,g,b = mappy.getValue(mappy.PALETTEARGB+i)
				out:write( a .. "," .. r .. "," .. g .. "," .. b )
				i = i + 1
				if ( i ~= 256 ) then
					out:write( "," )
				end
				out:write( "\n" )
			end
			out:write( "}\n" )
		end

		local blockw = getValue("BLOCKWIDTH")
		local blockh = getValue("BLOCKHEIGHT")
		local blocks = getValue("NUMBLOCKGFX")
		out:write( "\n" )
		out:write( "BLOCKBITMAPS={\n" )
		i = 0
		while ( i < blocks ) do
			local y = 0
			while ( y < blockh ) do
				out:write( "    " )
				local x = 0
				while ( x < blockw ) do
					if ( blockbpp == 8 ) then
						local a = mappy.getPixel( x, y, i )
						out:write( a )
					else
						local a,r,g,b = mappy.getPixel( x, y, i )
						out:write( r .. "," .. g .. "," .. b .. "," .. a )
					end
					if ( x+1 ~= blockw or y+1 ~= blockh or i+1 ~= blocks ) then
						out:write( ", " )
					end
					x = x + 1
				end
				y = y + 1
				out:write( "\n" )
			end
			out:write( "\n" )
			i = i + 1
		end
		out:write( "}\n" )

		local mapw = getValue("MAPWIDTH")
		local maph = getValue("MAPHEIGHT")
		out:write( "\n" )
		out:write( "MAP={\n" )
		local y = 0
		while ( y < maph ) do
			out:write( "    " )
			local x = 0
			while ( x < mapw ) do
				local ix = mappy.getBlock( x, y )
				out:write( ix )
				if ( x+1 ~= mapw or y+1 ~= maph ) then
					out:write( "," )
				end
				x = x + 1
			end
			y = y + 1
			out:write( "\n" )
		end
		out:write( "}\n" )

		out:close()
	end
	return nil
end

local errormsg = main()
if errormsg ~= nil then
	mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
