/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/




package VjComponents.ConfigEditor;

import java.lang.String;
import java.lang.StringBuffer;
import java.util.*;
import java.io.*;
import javax.swing.*;

import VjConfig.*;
import VjControl.*;
import VjComponents.ConfigEditor.ConfigCommunicator;


/** VjControl core module for viewing/editing config & ChunkDesc files.
 *  This module stores a set of ConfigChunkDB objects and a set of
 *  ChunkDescDB objects.  By cooperating with the network module, some
 *  of these ConfigChunkDBs may represent the state of running VR Juggler
 *  applications.  The rest simply represent files.
 *  <p>
 *  The ConfigChunkDBs and ChunkDescDBs are edited by a number of vjcontrol
 *  panels.
 *  <p>
 *  Note that, although the ConfigModule gives access to these DBs directly,
 *  they should be treated as <b>read only</b>.  Use 
 *  ConfigModule.addChunk (ConfigChunkDB, ConfigChunk) and related functions
 *  to change the DBs.  This will automagically take care of whether the DB
 *  is a file or network connection.
 */
public class ConfigModule extends DefaultCoreModule {

    static public Vector chunkdbs;
    static protected Vector descdbs;

    /** When this descdb is created, it subsumes Core.descdb's contents. 
     *  Actually, current implementation makes this an alias for Core.descdb.
     */
    static public ChunkDescDB descdb;

    /** Chunkdb associated with a ConfigCommunicator.
     *  active_chunkdb represents the state of whatever application
     *  (typically a VR Juggler program) is on the other end of the
     *  network connection.  By modifying it, we can reconfigure the
     *  application.
     *  <p>
     *  It is important that any modifications to active_chunkdb are
     *  performed thru the ConfigCommunicator interface, so that the
     *  application can be modified appropriately.  The app will then
     *  send updated configuration information, which the Communicator
     *  will use to update the active_chunkdb itself.
     *  <p>
     *  Currently, the ConfigModule only handles a single networked
     *  chunkdb, but that limitation is at least hidden from the UI
     *  components.
     */
    static protected ConfigChunkDB active_chunkdb;

    /** DescDB associated with a ConfigCommunicator.
     *  analogous to active_chunkdb.  Descriptions inserted into 
     *  active_descdb are also added to VjControl's global descdb for
     *  use by the ChunkFactory object.
     */
    static protected ChunkDescDB active_descdb;

    /** Targets to notify when a chunkdb or descdb is added or removed */
    static private Vector configmodule_targets;

    static final private int ADD_CHUNKDB = 1;
    static final private int REMOVE_CHUNKDB = 2;
    static final private int ADD_DESCDB = 3;
    static final private int REMOVE_DESCDB = 4;


    /** ConfigCommunicator which is currently servicing a connection.
     *  This variable is set via the communicator itself calling
     *  initConnection, and so does not constitute a dependency on
     *  the ConfigModule's instantiation.
     */
    protected ConfigCommunicator net;



    /** Constructor */
    public ConfigModule () {
        super();
        chunkdbs = new Vector();
        descdbs = new Vector();
        // is this a really bad idea?
        descdb = Core.descdb;
        //descdb = new ChunkDescDB();
        //descdb.addElements (Core.descdb);
        //descdb.setName ("ConfigModule Global ChunkDescDB");
        //ChunkFactory.setChunkDescDB (descdb);

        configmodule_targets = new Vector();

        active_chunkdb = null;
        active_descdb = null;
        net = null;
    }



    /** Configure the ConfigModule.
     *  This is complicated because this is where we do all the initial
     *  autoloading of files specified in VjControl's global prefs, 
     *  this component's own chunk, and on the vjcontrol command line.
     *  whew!
     */ 
    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        List descdbnames = new ArrayList();
        List chunkdbnames = new ArrayList();
        List auto_descdbnames = new ArrayList();
        List auto_chunkdbnames = new ArrayList();
        boolean autoload = true;
        String lastdname = "";
        String lastfname = "";
        int i, j, m, n;
        ConfigChunk chunk;
        Property p;
        String s;

        component_name = ch.getName();

        // check command-line arguments stored in Core
        String[] args = Core.getCommandLine();
	for (i = 0; i < args.length; i++) {
	    if (args[i].startsWith ("-d")) {
                if (args[i].length() == 2)
                    descdbnames.add (args[++i]);
                else
                    descdbnames.add (args[i].substring(2));
	    }
            else if (args[i].endsWith(".desc") || args[i].endsWith(".DESC"))
                descdbnames.add (args[i]);
	    else if (args[i].startsWith ("-c")) {
                if (args[i].length() == 2)
                    chunkdbnames.add (args[++i]);
                else
                    chunkdbnames.add (args[i].substring(2));
	    }
            else if (args[i].endsWith(".config") || args[i].endsWith(".CONFIG"))
                chunkdbnames.add (args[i]);
	    else if (args[i].equalsIgnoreCase ("-noautoload")) {
		autoload = false;
	    }
        }

        // get autoload info from vjc's basic configchunk
	List v = Core.vjcontrol_chunkdb.getOfDescToken ("vjcontrol");
        n = v.size();
        for (i = 0; i < n; i++) {
            chunk = (ConfigChunk)v.get(i);
            p = chunk.getPropertyFromToken ("descfiles");
            m = p.getNum();
            for (j = 0; j < m; j++) {
                s = p.getValue(j).getString();
                auto_descdbnames.add (s);
            }
            p = chunk.getPropertyFromToken ("chunkfiles");
            m = p.getNum();
            for (j = 0; j < m; j++) {
                s = p.getValue(j).getString();
                auto_chunkdbnames.add (s);
            }
        }

        // and (eventually) from ConfigModule's configchunk...
        
        // autoload initial files

	if (autoload) {
            n = auto_descdbnames.size();
	    for (i = 0; i < n; i++) {
		lastdname = (String)auto_descdbnames.get(i);
		loadNewDescDBFile (lastdname);
	    }
	}
        n = descdbnames.size();
	for (i = 0; i < n; i++) {
	    lastdname = (String)descdbnames.get(i);
	    loadNewDescDBFile (lastdname);
	}
	if (autoload) {
            n = auto_chunkdbnames.size();
	    for (i = 0; i < n; i++) {
		lastfname = (String)auto_chunkdbnames.get(i);
		loadNewChunkDBFile (lastfname);
	    }
	}
        n = chunkdbnames.size();
	for (i = 0; i < n; i++) {
	    lastfname = (String)chunkdbnames.get(i);
	    loadNewChunkDBFile (lastfname);
	}
    }


    public void initialize () throws VjComponentException {
        // strictly, loading should happen here & not configure.
    }


//      public void destroy () {
//          ;
//      }


    //-------------------- ConfigChunkDB Management -------------------------


    /** Adds a ConfigChunkDB to self.
     *  Once _chunkdb is added, it becomes the property of self, and
     *  <b>must not</b> be edited by any other object, except using
     *  the methods provided by ConfigModule (ConfigModule.addChunk (db, ch)
     *  and so forth).
     *  <p>
     *  The one exception to this is that the ConfigCommunicator can
     *  (and has to) edit the active_chunkdb.
     */
    public String addChunkDB (ConfigChunkDB _chunkdb) {
	_chunkdb.setName (createUniqueChunkDBName (_chunkdb.name));
	chunkdbs.add (_chunkdb);
	notifyConfigModuleTargets (ADD_CHUNKDB, _chunkdb, null);
	return _chunkdb.name;
    }



    /** Removes a ChunkDB from the system */
    public void closeChunkDB (ConfigChunkDB db) {
        if (db == null)
            return;
	notifyConfigModuleTargets (REMOVE_CHUNKDB, db, null);
	chunkdbs.remove (db);
    }


    public String renameChunkDB (ConfigChunkDB _db, String newbase) {
	if (_db.name.equals (newbase))
	    return newbase;
	newbase = createUniqueChunkDBName (newbase);
	notifyConfigModuleTargets (REMOVE_CHUNKDB, _db, null);
	_db.setName (newbase);
	notifyConfigModuleTargets (ADD_CHUNKDB, _db, null);
	return newbase;
    }



    public ConfigChunkDB getChunkDB (String name) {
	ConfigChunkDB db;
	for (int i = 0; i < chunkdbs.size(); i++) {
	    db = (ConfigChunkDB)chunkdbs.get(i);
	    if (db.getName().equalsIgnoreCase(name))
		return db;
	}
	return null;
    }


    public ConfigChunkDB getChunkDB (File f) {
	ConfigChunkDB db;
	for (int i = 0; i < chunkdbs.size(); i++) {
	    db = (ConfigChunkDB)chunkdbs.get(i);
            if (db.getFile().equals (f))
                return db;
	}
	return null;
    }
    


    public boolean isActive (ConfigChunkDB db) {
        return (db == active_chunkdb);
    }


    protected String createUniqueChunkDBName (String base) {
	// returns a string, starting with base, which doesn't
	// conflict with names of any dbs in chunkdbs.
	int i;
	String name;

	if (getChunkDB (base) == null)
	    return base;
	for (i = 2; true; i++) {
	    name = base + " <" + i + ">";
	    if (getChunkDB (name) == null)
		return name;
	}
    }


    //----------------- ConfigChunkDB Editing Stuff --------------------------

    public ConfigChunk findPrefixMatchChunk (String name) {
	/* finds a chunk whose name is a prefix of name */
	ConfigChunk ch;
	ConfigChunkDB db;
	int i, j, n, m;
        n = chunkdbs.size();
	for (i = 0; i < n; i++) {
	    db = (ConfigChunkDB)chunkdbs.get(i);
            m = db.size();
	    for (j = 0; j < m; j++) {
		ch = db.get(j);
		if (name.startsWith (ch.getName()))
		    return ch;
	    }
	}
	return null;
    }



   /** Adds a chunk to a ConfigChunkDB.
     *  @param db ConfigChunkDB to add to.
     *  @param ch ConfigChunk to add.
     *  @return True if the ConfigChunk was immediately added (ie db was a
     *          local file) and false if the addition was delayed (ie db is
     *          an "active config" associated with a network connection). 
     *          In the former case, a GUI component calling addChunk should
     *          update immediately; in the former case, it should wait for
     *          an event.
     */
    public boolean addChunk (ConfigChunkDB db, ConfigChunk ch) {
        if (db == active_chunkdb) {
            net.sendChunk (ch);
            return false;
        }
        else {
            db.add (ch);
            return true;
        }
    }


    public boolean addChunks (ConfigChunkDB db, ConfigChunkDB chunks) {
        if (db == active_chunkdb) {
            net.sendChunks (chunks);
            return false;
        }
        else {
            db.addAll (chunks);
            return true;
        }
    }


    public boolean removeChunk (ConfigChunkDB db, ConfigChunk chunk) {
        if (db == active_chunkdb) {
            net.removeChunk (chunk);
            return false;
        }
        else {
            db.remove (chunk);
            return true;
        }
    }


    public boolean removeChunks (ConfigChunkDB db, ConfigChunkDB chunks) {
        if (db == active_chunkdb) {
            net.removeChunks (chunks);
            return false;
        }
        else {
            db.removeAll (chunks);
            return true;
        }
    }


    public boolean replaceChunk (ConfigChunkDB db, ConfigChunk oldc, ConfigChunk newc) {
        if (db == active_chunkdb) {
            if (!oldc.getName().equals(newc.getName()))
                net.removeChunk (oldc);
            net.sendChunk (newc);
            return false;
        }
        else {
            db.replace (oldc, newc);
            return true;
        }
    }



    //------------------- ChunkDB Utility Fns ----------------------

    /** Returns an array containing names of all ChunkDBs in self. */
    public String[] getChunkDBNames () {
        int n = chunkdbs.size();
        String[] names = new String[n];
        for (int i = 0; i < n; i++)
            names[i] = ((ConfigChunkDB)chunkdbs.get(i)).getName();
        return names;
    }



    /** Returns an array containing all ChunkDBs in self. 
     *  This array should only be considered valid for short-term
     *  operations; objects should be ConfigModule listeners if they
     *  need to maintain up-to-date information. 
     */
    public ConfigChunkDB[] getChunkDBs () {
        int n = chunkdbs.size();
        ConfigChunkDB[] dbs = new ConfigChunkDB[n];
        for (int i = 0; i < n; i++)
            dbs[i] = (ConfigChunkDB)chunkdbs.get(i);
        return dbs;
    }



    //------------------- ChunkDescDB Management -----------------------------

    
    /** Adds a ChunkDescDB to self.
     *  Once _descdb is added, it becomes the property of self, and
     *  <b>must not</b> be edited by any other object, except using
     *  the methods provided by ConfigModule (ConfigModule.addDesc (db, ch)
     *  and so forth).
     *  <p>
     *  The one exception to this is that the ConfigCommunicator can
     *  (and has to) edit the active_descdb.
     */
    public String addDescDB (ChunkDescDB _descdb) {
	_descdb.setName(createUniqueDescDBName (_descdb.name));
	descdbs.add ( _descdb);
	descdb.addAll (_descdb);
	notifyConfigModuleTargets (ADD_DESCDB, null, _descdb);
	return _descdb.name;
    }



    /** Removes a DescDB from the system */
    public void closeDescDB (ChunkDescDB db) {
	if (db == null || db.name.equalsIgnoreCase ("No Selection") || db == descdb)
	    return;
	notifyConfigModuleTargets (REMOVE_DESCDB, null, db);
	descdbs.remove (db);
    }



    public ChunkDescDB getDescDB (File f) {
        ChunkDescDB db;
        File canonical, other, other_canonical;
        try {
            canonical = f.getCanonicalFile();
        }
        catch (IOException e1) {
            canonical = f;
        }
	for (int i = 0; i < descdbs.size(); i++) {
            db = (ChunkDescDB)descdbs.get(i);
	    other = db.getFile();
            try {
                other_canonical = other.getCanonicalFile();
            }
            catch (IOException e2) {
                other_canonical = other;
            }
            if (canonical.equals (other_canonical))
                return db;
	}
	return null;
    }


    public boolean isActive (ChunkDescDB db) {
        return (db == active_descdb);
    }


    protected String createUniqueDescDBName (String base) {
	int i;
	String name;
	if (getChunkDescDB (base) == null)
	    return base;
	for (i = 2; true; i++) {
	    name = base + " <" + i + ">";
	    if (getChunkDescDB (name) == null)
		return name;
	}
    }



    public String renameDescDB (ChunkDescDB _db, String newbase) {
	if (_db.name.equals (newbase))
	    return newbase;
	newbase = createUniqueDescDBName (newbase);
	notifyConfigModuleTargets (REMOVE_DESCDB, null, _db);
	_db.setName (newbase);
	notifyConfigModuleTargets (ADD_DESCDB, null, _db);
	return newbase;
    }



    public ChunkDescDB getChunkDescDB (String name) {
	ChunkDescDB db;
	for (int i = 0; i < descdbs.size(); i++) {
	    db = (ChunkDescDB)descdbs.get(i);
	    if (db.name.equalsIgnoreCase(name))
		return db;
	}
	return null;
    }



    //--------------------- DescDB Editing Stuff -----------------------------


    /** Adds a ChunkDesc to a ChunkDescDB.
     *  @param db ChunkDescDB to add to.
     *  @param desc ChunkDesc to add.
     *  @return True if the ChunkDesc was immediately added (ie db was a
     *          local file) and false if the addition was delayed (ie db is
     *          an "active config" associated with a network connection). 
     *          In the former case, a GUI component calling addDesc should
     *          update immediately; in the former case, it should wait for
     *          an event.
     */
    public boolean addDesc (ChunkDescDB db, ChunkDesc desc) {
        if (db == active_descdb) {
            //net.sendChunk (ch);
            return false;
        }
        else {
            db.add (desc);
            return true;
        }
    }



    public boolean removeDesc (ChunkDescDB db, ChunkDesc desc) {
        if (db == active_descdb) {
            //net.removeChunk (chunk);
            return false;
        }
        else {
            db.remove (desc);
            return true;
        }
    }



    public boolean replaceDesc (ChunkDescDB db, ChunkDesc olddesc, ChunkDesc newdesc) {
        if (db == active_descdb) {
            // currently no live editing of juggler's descdb; this is pretend.
            // net.removeDesc (old_desc);
            // net.sendDesc (new_desc);
            return false;
        }
        else {
            db.replace (olddesc, newdesc);
            return true;
        }
    }




    //-------------------- DescDBUtility Fns ----------------------

    /** Returns the names of all ChunkDescs defined in any of our DBs.
     *  This is useful for GUI components that want to provide a list
     *  or menu of desc names.
     */
    public String[] getDescNames () {
        int n = descdb.size();
        String[] names = new String[n];
        for (int i = 0; i < n; i++)
            names[i] = descdb.get(i).getName();
        return names;
    }


    /** Returns an array containing the names of all DescDBs in self. */
    public String[] getDescDBNames () {
        int n = descdbs.size();
        String[] names = new String[n];
        for (int i = 0; i < n; i++)
            names[i] = ((ChunkDescDB)descdbs.get(i)).getName();
        return names;
    }



    /** Returns an array containing all DescDBs in self. 
     *  This array should only be considered valid for short-term
     *  operations; objects should be ConfigModule listeners if they
     *  need to maintain up-to-date information. 
     */
    public ChunkDescDB[] getDescDBs () {
        int n = descdbs.size();
        ChunkDescDB[] dbs = new ChunkDescDB[n];
        for (int i = 0; i < n; i++)
            dbs[i] = (ChunkDescDB)descdbs.get(i);
        return dbs;
    }



    /** Maps a ChunkDesc name to a token */
    public String getDescTokenFromName (String name) {
        ChunkDesc d = descdb.getByName (name);
        return (d == null)? null: d.getToken();
    }



    //--------------------- ConfigModuleListener Stuff -----------------------


    public synchronized void addConfigModuleListener (ConfigModuleListener l) {
	synchronized (configmodule_targets) {
	    configmodule_targets.add (l);
	}
    }

    public void removeConfigModuleListener (ConfigModuleListener l) {
	synchronized (configmodule_targets) {
	    configmodule_targets.remove (l);
	}
    }

    protected void notifyConfigModuleTargets (int msgtype, ConfigChunkDB _chunkdb, ChunkDescDB _descdb) {
	Vector l;
	ConfigModuleEvent e = new ConfigModuleEvent (this, _chunkdb, _descdb);
	synchronized (configmodule_targets) {
	    l = (Vector) configmodule_targets.clone();
	}
	for (int i = 0; i < l.size(); i++) {
	    ConfigModuleListener lis = (ConfigModuleListener)l.elementAt (i);
	    switch (msgtype) {
	    case ADD_CHUNKDB:
		lis.chunkDBAdded (e);
		break;
	    case REMOVE_CHUNKDB:
		lis.chunkDBRemoved (e);
		break;
	    case ADD_DESCDB:
		lis.descDBAdded (e);
		break;
	    case REMOVE_DESCDB:
		lis.descDBRemoved (e);
		break;
	    }
	}
    }




    //------------------------ Config File Methods ---------------------------



    // this is used specifically to load into an existing chunkdb without
    // adding anything into the gui's list of chunkdb's... useful
    // for loading the default chunkdb and keeping it hidden
    protected boolean loadChunkDBFileInto (ConfigChunkDB chunkdb, String currentdir) {
	currentdir = Core.file.mangleFileName (currentdir);

	File f = null;
	FileReader r;
	ConfigStreamTokenizer st;
	boolean retval = true;
	try {
	    f = new File (currentdir);
	    r = new FileReader (f);
	    st = new ConfigStreamTokenizer(r);
	    chunkdb.setName(f.getName());
	    chunkdb.setFile(f);
	    chunkdb.read(st);
	    List v = chunkdb.getOfDescToken("vjIncludeFile");
	    for (int i = 0; i < v.size(); i++)
		retval = retval && loadChunkDBFileInto(chunkdb, ((ConfigChunk)v.get(i)).getName());
            chunkdb.need_to_save = true;
	    return retval;
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage (component_name, "File Not Found: " + f);
	    return false;
	}

    }



    public String loadNewChunkDBFile (String filename) {
	filename = Core.file.mangleFileName (filename);
        File f = new File (filename);
        return loadNewChunkDBFile (f);
    }



    public String loadNewChunkDBFile (File f) {
        if (f == null)
            return null;

        ConfigChunkDB chunkdb;
        boolean is_new_db = true;

        // check if it's already loaded
        chunkdb = getChunkDB (f);
        if (chunkdb == null) {
            chunkdb = new ConfigChunkDB();
        }
        else {
            // if this file has already been loaded once, we need to
            // decide wether to revert to saved or not.

            boolean should_revert = false;
            if (java.beans.Beans.isGuiAvailable()) {
                int result = 
                    JOptionPane.showConfirmDialog (
                        null, "Do you want to revert to the saved '" + 
                        chunkdb.getName() + "'?", 
                        "Revert file?", 
                        JOptionPane.YES_NO_OPTION, 
                        JOptionPane.QUESTION_MESSAGE
                        /*, Core.save_icn*/);
                if (result == JOptionPane.YES_OPTION)
                    should_revert = true;
            }

            if (should_revert) {
                // get rid of old contents & load in new
                chunkdb.clear();
                is_new_db = false;
            }
            else {
                // don't do anything; just return the found db
                return chunkdb.getName();
            }
        }                


	Core.consoleInfoMessage (component_name, "Loading ChunkDB: " + f);
        try {
            chunkdb.setName(f.getName());
            chunkdb.setFile(f);
            ConfigIO.readConfigChunkDB (f, chunkdb, ConfigIO.GUESS);
            if (is_new_db) 
                addChunkDB (chunkdb);

            chunkdb.need_to_save = false;
	    
            // load included files...
            List v = chunkdb.getOfDescToken("vjIncludeFile");
            for (int i = 0; i < v.size(); i++)
                loadNewChunkDBFile(((ConfigChunk)v.get(i)).getName());

            return chunkdb.name;
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage (component_name, e.getMessage());
	    return null;
	}
        catch (IOException e) {
            Core.consoleErrorMessage (component_name, "Parsing error: " + e);
            return null;
        }
        catch (ConfigParserException e) {
            Core.consoleErrorMessage (component_name, "Parsing error: " + e.getMessage());
            return null;
        }
    }



    public String saveChunkDBFile (ConfigChunkDB db, File f) {
	if (f == null)
	    return db.name;

	try {
	    DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
            ConfigIO.writeConfigChunkDB (out, db, ConfigIO.DEFAULT);
	    Core.consoleInfoMessage (component_name, "Saved ChunkDB file: " + f);
            db.need_to_save = false;
	    /* do some fixing up if the name changed */
	    db.setFile (f);
            renameChunkDB (db, f.getName());

//             String newname = db.getFile().getName();
//             if (!newname.equals(db.getName())) {
//                 db.setName (createUniqueChunkDBName(newname));
//             }
            return db.name;
	}
	catch (IOException e) {
	    Core.consoleErrorMessage (component_name, "IOerror saving file " + f);
	    return db.name;
	}
    }



    //------------------------ DescDB File Methods ---------------------------



    public String loadNewDescDBFile (String currentdir) {
	currentdir = Core.file.mangleFileName (currentdir);
        File f = new File (currentdir);
        return loadNewDescDBFile (f);
    }


    public String loadNewDescDBFile (File f) {
        if (f == null)
            return null;

        ChunkDescDB descdb;
        boolean is_new_db = true;

        // check if it's already loaded
        descdb = getDescDB (f);
        if (descdb == null) {
            descdb = new ChunkDescDB();
        }
        else {
            // if this file has already been loaded once, we need to
            // decide wether to revert to saved or not.

            boolean should_revert = false;
            if (java.beans.Beans.isGuiAvailable()) {
                int result = 
                    JOptionPane.showConfirmDialog (
                        null, "Do you want to revert to the saved '" + 
                        descdb.getName() + "'?", 
                        "Revert file?", 
                        JOptionPane.YES_NO_OPTION, 
                        JOptionPane.QUESTION_MESSAGE
                        /*, Core.save_icn*/);
                if (result == JOptionPane.YES_OPTION)
                    should_revert = true;
            }

            if (should_revert) {
                // get rid of old contents & load in new
                descdb.clear();
                is_new_db = false;
            }
            else {
                // don't do anything; just return the found db
                return descdb.getName();
            }
        }                


	Core.consoleInfoMessage (component_name, 
				 "Loading Descriptions file: " + f);
 	try {
	    descdb.setName(f.getName());
	    descdb.setFile (f);
            ConfigIO.readChunkDescDB (f, descdb, ConfigIO.GUESS);
            descdb.need_to_save = false;
            if (is_new_db)
                addDescDB (descdb);
            return descdb.name;
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage (component_name, e.getMessage());
	    return null;
	}
        catch (IOException e) {
            Core.consoleErrorMessage (component_name, "Parsing error: " + e);
            return null;
        }
        catch (ConfigParserException e) {
            Core.consoleErrorMessage (component_name, "Parsing error: " + e.getMessage());
            return null;
        }
    }


    public String saveDescDBFile (ChunkDescDB db, File f) {
	if (f == null)
	    return db.name;
	
	try {
	    DataOutputStream out = 
		new DataOutputStream(new FileOutputStream(f));
            if (ConfigIO.writeChunkDescDB (out, db, ConfigIO.DEFAULT)) {
                db.need_to_save = false;
                Core.consoleInfoMessage (component_name, 
                                         "Saved ChunkDesc File: " + f);
                /* do some fixing up if the name changed */
                db.setFile (f);
                renameDescDB (db, f.getName());
            }
            else {
                Core.consoleErrorMessage (component_name, "Save failed: " +f);
            }
            return db.name;
	}
	catch (IOException e) {
	    Core.consoleErrorMessage (component_name, "IOerror saving file " + f);
	    return db.name;
	}
    }



    //-------------------- Network Connection Mgmt Stuff --------------------

    /** Called when ConfigCommunicator opens a new Connection.
     *  This should not be called if the ConfigCommunicator thinks it
     *  already has a set of active DBs.
     */
    public void initConnection (ConfigCommunicator cm, ChunkDescDB descdb,
                                ConfigChunkDB chunkdb) {
        if (net != null) {
            Core.consoleErrorMessage (component_name, "Can only handle one connection!");
            return;
        }
        net = cm;
        active_chunkdb = chunkdb;
        active_descdb = descdb;
        addDescDB (descdb);
        addChunkDB (chunkdb);
    }



    public void shutdownConnection (ConfigCommunicator cm) {
        if (net == null) {
            Core.consoleErrorMessage (component_name, "shutdownConnection called, but there was no connection at all.");
            return;
        }
        closeChunkDB (active_chunkdb);
        closeDescDB (active_descdb);
        net = null;
    }



}
