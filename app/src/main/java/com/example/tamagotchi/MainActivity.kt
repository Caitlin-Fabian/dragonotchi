package com.example.tamagotchi

import android.graphics.Color
import android.graphics.drawable.ColorDrawable
import android.os.Bundle

import android.util.Log
import android.view.Menu
import android.view.MenuItem
import com.google.android.material.bottomnavigation.BottomNavigationView
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.NavController
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.example.tamagotchi.databinding.ActivityMainBinding
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.database.ktx.getValue
import com.google.firebase.ktx.Firebase

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val navView: BottomNavigationView = binding.navView

        val navController = findNavController(R.id.nav_host_fragment_activity_main)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.


        val appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.navigation_home, R.id.navigation_dashboard, R.id.navigation_notifications
            )
        )

        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)

        val actionBar = supportActionBar

        // providing title for the ActionBar
        actionBar!!.title = ""
        actionBar.setBackgroundDrawable(ColorDrawable(Color.TRANSPARENT))

        // providing subtitle for the ActionBar
        actionBar.subtitle = ""

        // adding icon in the ActionBar
        //actionBar.setIcon()

        // methods to display the icon in the ActionBar
        actionBar.setDisplayUseLogoEnabled(true)
        actionBar.setDisplayShowHomeEnabled(true)
    }

    // method to inflate the options menu when
    // the user opens the menu for the first time
    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        val database = Firebase.database

        checkHealth(database){health ->
            for (i in 0 until health) {
            val menuItem = menu.add(Menu.NONE, Menu.NONE, i, "Heart")
            menuItem.setIcon(R.drawable.life)
            menuItem.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS)
        }
        }


        menuInflater.inflate(R.menu.toolbar_menu, menu)


        return super.onCreateOptionsMenu(menu)
    }

        }


    fun checkExistence( database: FirebaseDatabase, callback: (Boolean) -> Unit) {
        val dragonExists = database.getReference("dragonExists")

        dragonExists.addListenerForSingleValueEvent(object: ValueEventListener {

            override fun onDataChange(snapshot: DataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                val value = snapshot.getValue<Boolean>()
                Log.d("FireBase", "Value is: " + value)
                callback(value ?: false)

            }

            override fun onCancelled(error: DatabaseError) {
                Log.w("FireBase", "Failed to read value.", error.toException())
                callback(false) // Handle the error by providing a default value

            }

        })


    }

    fun checkHealth(database: FirebaseDatabase, callback: (Int) -> Unit) {
        val health = database.getReference("health")

        health.addListenerForSingleValueEvent(object: ValueEventListener {

            override fun onDataChange(snapshot: DataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                val value = snapshot.getValue<Int>()
                Log.d("FireBase", "Value is: " + value)
                if (value != null) {
                    callback(value)
                }

            }

            override fun onCancelled(error: DatabaseError) {
                Log.w("FireBase", "Failed to read value.", error.toException())
                callback(0) // Handle the error by providing a default value

            }

        })

    }

//
//        // Write a message to the database
//        val database = Firebase.database
//        val healthRef = database.getReference("health")
//        val boredomRef = database.getReference("boredomLevel")
//        val cleanlinessRef = database.getReference("cleanlinessLevel")
//        val dragonExists = database.getReference("dragonExists")
//
//        dragonExists.setValue(false)
//        healthRef.setValue("5")
//        boredomRef.setValue("5")
//        cleanlinessRef.setValue("5")
//
//
//
//
//
//

//
//        // Read from the database
//        cleanlinessRef.addValueEventListener(object: ValueEventListener {
//
//            override fun onDataChange(snapshot: DataSnapshot) {
//                // This method is called once with the initial value and again
//                // whenever data at this location is updated.
//                val value = snapshot.getValue<String>()
//                Log.d("FireBase", "Value is: " + value)
//            }
//
//            override fun onCancelled(error: DatabaseError) {
//                Log.w("FireBase", "Failed to read value.", error.toException())
//            }
//
//        })
//
//        // Read from the database
//        boredomRef.addValueEventListener(object: ValueEventListener {
//
//            override fun onDataChange(snapshot: DataSnapshot) {
//                // This method is called once with the initial value and again
//                // whenever data at this location is updated.
//                val value = snapshot.getValue<String>()
//                Log.d("FireBase", "Value is: " + value)
//            }
//
//            override fun onCancelled(error: DatabaseError) {
//                Log.w("FireBase", "Failed to read value.", error.toException())
//            }
//
//        })
//
//        // calling this activity's function to
//        // use ActionBar utility methods
//        val actionBar = supportActionBar
//        // providing title for the ActionBar
//        actionBar!!.title = ""
//
//        // providing subtitle for the ActionBar
//        actionBar.subtitle = ""
//
//        // adding icon in the ActionBar
//        //actionBar.setIcon()
//
//        // methods to display the icon in the ActionBar
//        actionBar.setDisplayUseLogoEnabled(true)
//        actionBar.setDisplayShowHomeEnabled(true)
//    }

