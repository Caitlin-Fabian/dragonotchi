package com.example.tamagotchi.ui.notifications
import androidx.compose.foundation.Image
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater

import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material.Text
import androidx.compose.material.Button
import androidx.compose.material.TextButton

import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.ComposeView
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.tamagotchi.R
import com.example.tamagotchi.databinding.FragmentNotificationsBinding
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase

class NotificationsFragment : Fragment() {

    private var _binding: FragmentNotificationsBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        val notificationsViewModel =
            ViewModelProvider(this).get(NotificationsViewModel::class.java)

        _binding = FragmentNotificationsBinding.inflate(inflater, container, false)
        val root: View = binding.root

        val textView: TextView = binding.textNotifications
        notificationsViewModel.text.observe(viewLifecycleOwner) {
            textView.text = it

        }
        val rootView = inflater.inflate(R.layout.fragment_notifications, container, false)

        val composeView = rootView.findViewById<ComposeView>(R.id.composeView)

        composeView.setContent {
            // Define your Composable content here
            CleanUp()
        }


        return rootView
    }

    @Preview
    @Composable
    fun CleanUp() {
        val configuration = LocalConfiguration.current
        val screenHeight = configuration.screenHeightDp.dp
        val screenWidth = configuration.screenWidthDp.dp
        Column(
            modifier = Modifier.height(screenHeight).width(screenWidth),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally
        ) {

            TextButton( onClick = {
                val database = Firebase.database
                val cleanRef = database.getReference("cleanlinessLevel")
                val mopRef = database.getReference("usedMop")

                // attempt to get and update cleanliness level
                cleanRef.get().addOnSuccessListener {
                    Log.i("firebase", "Got value ${it.value}")
                    var myInt = it.value.toString().toInt()
                    if (myInt in 0..4) {
                        myInt++
                        mopRef.setValue(true)
                        cleanRef.setValue(myInt)
                    }
                }.addOnFailureListener {
                    Log.e("firebase", "Error getting data", it)
                }
            }) {
                Image(
                    modifier = Modifier.size(300.dp),
                    painter = painterResource(R.drawable.mop_drawing),
                    contentDescription = "Contact profile picture",
                )
            }
            Text("Clean up the mess ")

        }


    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}